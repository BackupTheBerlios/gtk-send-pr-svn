/*
  Copyright (c) 2003, 2004, 2005 Miguel Mendez <mmendez@energyhq.be>

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
					
#ifndef _PR_DEFS_H_
#define _PR_DEFS_H_

typedef struct pr_defs {

  char **pr_categories;
  int  cat_num;

  char **pr_severities;
  int  sev_num;

  char **pr_priorities;
  int  pri_num;

  char **pr_classes;
  int class_num;

  char *default_to;
  char *default_rcpt;
  char *default_sub_id;

} PR_DEFS;

#if defined(__FreeBSD__) || defined(__DragonFly__)

#define MAINT_CLASS "maintainer-update"
#define MAINT_CAT "ports"

#else

#define MAINT_CLASS "change-request"
#define MAINT_CAT "pkg"

#endif

void show_categories(void);
int fill_prdefs(PR_DEFS *);

#endif
