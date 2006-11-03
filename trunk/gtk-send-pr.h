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

#ifndef _GTK_SEND_PR_H_
#define _GTK_SEND_PR_H_

#define GSP_VERSION "0.4.8"
#define GSP_CODENAME "\"Time What Is Time\""

#define GSP_SSL_NO 0
#define GSP_SSL_EN 1
#define GSP_SSL_RE 2

typedef struct user_prfile {

  char name[256];
  char email[256];
  char org[256];
  char smtp[256];
  char smtp_port[16];
  int geom_x;
  int geom_y;
  int  ssl_mode;
	
} USER_PROFILE;

typedef struct problem_report {

  char *smtp_server;
  char *smtp_port;
  char *smtp_from;
  char *smtp_to;
  char *smtp_rcpt;
  char *smtp_cc[64];
  char *smtp_cc_text;
  int  smtp_cc_num;
  char *smtp_subject;
  char *smtp_msgid;
  char *submitter_id;
  char *originator;
  char *organization;
  char *synopsis;
  char *severity;
  char *priority;
  char *category;
  char *class;
  char *release;
  char *environment;
  char *description;
  char *how_to_repeat;
  char *fix;
  int  ssl_mode;

} PROBLEM_REPORT;


typedef struct gsp_auth {

  char *username;
  char *password;

} GSP_AUTH;

#define MAINT_NO 0
#define MAINT_YES 1

struct file_element {

  char *filename;
  struct file_element *next;

};

typedef struct user_options {

  int  maint_mode;
  struct file_element *head;

} USER_OPTIONS;

#endif
