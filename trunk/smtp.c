/*
  Copyright (c) 2003 - 2006 Miguel Mendez <mmendez@energyhq.be>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 
  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution. 

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  $Id$

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <pwd.h>

#include <time.h>
extern char *tzname[2];

#include <openssl/ssl.h>
#include <auth-client.h>
#include <libesmtp.h>

#include "gtk-send-pr.h"
#include "gtk_ui.h"
#include "smtp.h"

#ifndef TRUE
#define TRUE 1
#endif

static void build_message(FILE *, PROBLEM_REPORT *);
static int authinteract(auth_client_request_t request, 
			char **result, int fields, void *arg);

static void event_cb(smtp_session_t session, 
		     int event_no, void *arg, ...);

char global_smtp_error_msg[1024];
static GSP_AUTH *my_auth = NULL;

extern int gsp_auth_done;

/*
 * Builds the message and tries to send it
 * via the specified SMTP server. Returns 0
 * if everything went ok, -1 if someting goes
 * south and sets `global_smtp_error_msg'.
 * If AUTH is required, a dialog will pop up
 * asking the user to authenticate.
 */
int
send_pr(PROBLEM_REPORT *mypr)
{
  smtp_session_t session;
  smtp_message_t message;
  smtp_recipient_t recipient;
  const smtp_status_t *status;
  auth_context_t authctx;
  int noauth = 0;
  char *host = NULL;
  int i = 0;

  enum notify_flags notify = Notify_SUCCESS|Notify_FAILURE;
  FILE *fp;
  char tempfile[FILENAME_MAX + 1];
  char *tmpdir;
  int  tempfd;

  char my_smtp_server[1024];
  char my_recipient[1024];
  char buf[128];

  if (gsp_auth_done != TRUE) {

    my_auth = malloc(sizeof(GSP_AUTH));
    my_auth->username = malloc(1024);
    memset(my_auth->username, 0, 1024);
    my_auth->password = malloc(1024);
    memset(my_auth->password, 0, 1024);

  }

  tmpdir = getenv("TMPDIR");

  memset(tempfile, 0, sizeof(tempfile));

  if (tmpdir != NULL) {

    snprintf(tempfile, FILENAME_MAX, "%s/gtk-send-pr.XXXXXXXX", tmpdir);

  } else {

    snprintf(tempfile, FILENAME_MAX , "/tmp/gtk-send-pr.XXXXXXXX");

  }

  tempfd = mkstemp(tempfile);
  fp = fdopen(tempfd, "w");

  build_message(fp, mypr);

  fclose(fp);

  auth_client_init();
  session = smtp_create_session();
  message = smtp_add_message(session);

  switch (mypr->ssl_mode) {

  case GSP_SSL_NO :

    i = smtp_starttls_enable(session, Starttls_DISABLED);
    break;

  case GSP_SSL_EN :

    i = smtp_starttls_enable(session, Starttls_ENABLED);
    break;

  case GSP_SSL_RE :

    i = smtp_starttls_enable(session, Starttls_REQUIRED);
    break;

  }

  snprintf(my_smtp_server, 1024, "%s:%s", mypr->smtp_server, mypr->smtp_port);

  host = my_smtp_server;

  smtp_set_server(session, host);

  authctx = auth_create_context();
  auth_set_mechanism_flags(authctx, AUTH_PLUGIN_PLAIN, 0);
  auth_set_interact_cb(authctx, authinteract, NULL);

  smtp_set_eventcb(session, event_cb, NULL);

  if (!noauth) {

    smtp_auth_set_context(session, authctx);

  }

  smtp_set_header(message, "From", mypr->originator, mypr->smtp_from);
  smtp_set_header(message, "To", mypr->smtp_rcpt, mypr->smtp_to);
  smtp_set_header(message, "Subject", mypr->smtp_subject);
  smtp_set_header(message, "Message-Id", NULL);
  smtp_set_reverse_path(message, mypr->smtp_from);
  
  fp = fopen(tempfile, "r");

  smtp_set_message_fp(message, fp);

  /* Recipient must be in RFC2821 format */
  snprintf(my_recipient, 1024, "%s", mypr->smtp_to);
  recipient = smtp_add_recipient(message, my_recipient);
  smtp_dsn_set_notify(recipient, notify);

  if (mypr->smtp_cc_num > 0) {

    for (i = 0; i < mypr->smtp_cc_num; i++) {

      smtp_set_header(message, "CC", NULL, mypr->smtp_cc[i]);
      snprintf(my_recipient, 1024, "%s", mypr->smtp_cc[i]);
      recipient = smtp_add_recipient(message, my_recipient);
      smtp_dsn_set_notify(recipient, notify);			

    }

  }

  if (!smtp_start_session(session)) {

    snprintf(global_smtp_error_msg, 1024, "SMTP server problem : %s\n",
	     smtp_strerror(smtp_errno(), buf, sizeof buf));

    status = smtp_message_transfer_status(message);
    smtp_destroy_session(session);
    auth_destroy_context(authctx);
    fclose(fp);
    unlink(tempfile);
    auth_client_exit();

    return(-1);

  } else {

    status = smtp_message_transfer_status(message);

    if (status != NULL) {

      /* Something went wrong... */
      if (status->code >= 400) {

	snprintf(global_smtp_error_msg, 1024, "SMTP server problem : %i %s\n",
		 status->code, status->text);

	smtp_destroy_session(session);
	auth_destroy_context(authctx);
	fclose(fp);
	unlink(tempfile);
	auth_client_exit();

	return(-1);

      }

    }

    smtp_destroy_session(session);
    auth_destroy_context(authctx);
    fclose(fp);
    unlink(tempfile);
    auth_client_exit();

    return(0);

  }

}

static void
build_message(FILE *fp, PROBLEM_REPORT *mypr)
{

  fprintf(fp, "Return-Path: <%s>\r\n", mypr->smtp_from);
  fprintf(fp, "Subject: %s\r\n", mypr->smtp_subject);
  fprintf(fp, "MIME-Version: 1.0\r\n");
  fprintf(fp, "Content-Type: text/plain;\r\n");
  fprintf(fp, "  charset=iso-8859-1\r\n");
  fprintf(fp, "Content-Transfer-Encoding: 7bit\r\n");
  fprintf(fp, "X-send-pr-version: gtk-send-pr " GSP_VERSION " \r\n");
  fprintf(fp, "X-GNATS-Notify: \r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, ">Submitter-Id:	%s \r\n", mypr->submitter_id);
  fprintf(fp, ">Originator:	%s \r\n", mypr->originator);
  fprintf(fp, ">Organization:	%s \r\n", mypr->organization);
  fprintf(fp, ">Confidential:	no \r\n");
  fprintf(fp, ">Synopsis:	%s \r\n", mypr->synopsis);
  fprintf(fp, ">Severity:	%s \r\n", mypr->severity);
  fprintf(fp, ">Priority:	%s \r\n", mypr->priority);
  fprintf(fp, ">Category:	%s \r\n", mypr->category);
  fprintf(fp, ">Class:		%s \r\n", mypr->class);
  fprintf(fp, ">Release:	%s \r\n", mypr->release);

  fprintf(fp, ">Environment:\r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, "%s\r\n", mypr->environment);
  fprintf(fp, "\r\n\r\n");

  fprintf(fp, ">Description:\r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, "%s\r\n", mypr->description);
  fprintf(fp, "\r\n\r\n");

  fprintf(fp, ">How-To-Repeat:\r\n");
  fprintf(fp, "\r\n\r\n");	
  fprintf(fp, "%s\r\n", mypr->how_to_repeat);
  fprintf(fp, "\r\n\r\n");

  fprintf(fp, ">Fix:\r\n");
  fprintf(fp, "\r\n\r\n");		
  fprintf(fp, "%s\r\n", mypr->fix);
  fprintf(fp, "\r\n\r\n");	

}

static int
authinteract(auth_client_request_t request, 
	     char **result, int fields, void *arg)
{
  int i;

  if (gsp_auth_done != TRUE) {

    gsp_smtp_auth_dialog(my_auth);

  }

  for (i = 0; i < fields; i++) {

    if (request[i].flags & AUTH_PASS) {

      result[i] = my_auth->password;

    } else if (request[i].flags & AUTH_USER) {

      result[i] = my_auth->username;

    } else if (result[i] == NULL) {

      return 0;

    }
  
  }

  return 1;

}

/* 
 * We currently just return 1 to keep libesmtp happy
 * Wil turn this into a proper event handler some day
 */
void 
event_cb(smtp_session_t session, int event_no, void *arg,...)
{
  va_list alist;
  int *ok;
  long foo;
  int bar;

  va_start(alist, arg);

  switch (event_no) {

  case SMTP_EV_WEAK_CIPHER:

    bar = va_arg(alist, long);
    ok = va_arg(alist, int*); 
    *ok = 1; 
    break;

  case SMTP_EV_INVALID_PEER_CERTIFICATE:

    foo = va_arg(alist, long);
    ok = va_arg(alist, int*); 
    *ok = 1; 
    break;

  case SMTP_EV_NO_PEER_CERTIFICATE:
  case SMTP_EV_WRONG_PEER_CERTIFICATE:
  case SMTP_EV_NO_CLIENT_CERTIFICATE:

    ok = va_arg(alist, int*); 
    *ok = 1; 
    break;

  case SMTP_EV_CONNECT: 
  case SMTP_EV_MAILSTATUS:
  case SMTP_EV_RCPTSTATUS:
  case SMTP_EV_MESSAGEDATA:
  case SMTP_EV_MESSAGESENT:
  case SMTP_EV_DISCONNECT: 
  default:

    break;

  }

  va_end(alist);

}
