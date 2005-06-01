/*
  Copyright (c) 2003-2004, Miguel Mendez. All rights reserved.

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

#define GSP_VERSION "0.3.2"
#define GSP_CODENAME "\"Pure Shoes\""

typedef struct user_prfile {

  char name[255];
  char email[255];
  char org[255];
  char smtp[255];
  int geom_x;
  int geom_y;
	
} USER_PROFILE;

typedef struct problem_report {

  char *smtp_server;
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
	
} PROBLEM_REPORT;


typedef struct gsp_auth {

  char *username;
  char *password;

} GSP_AUTH;
