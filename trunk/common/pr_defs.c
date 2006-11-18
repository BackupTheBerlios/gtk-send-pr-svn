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

#include "pr_defs.h"

#if defined(__FreeBSD__) || defined(__DragonFly__)

static char *pr_categories[]= { "advocacy", "alpha", "amd64", "bin", "conf",
				"docs", "gnu", "i386", "ia64", "java", "kern",
				"misc", "ports", "powerpc", "sparc64", "standards",
				"threads", "usb", "www"
};

static char *pr_severities[]= { "non-critical", "serious", "critical" };
static char *pr_priorities[]= { "low", "medium", "high" };
static char *pr_classes[]= { "sw-bug", "doc-bug", "change-request", "update", "maintainer-update"};
static char default_to[]="FreeBSD-gnats-submit@freebsd.org";
static char default_rcpt[]="FreeBSD gnats submit";
static char default_sub_id[]="current-users";

#else

static char *pr_categories[]= { "bin", "install", "kern", "lib", "misc", "pkg",
				"port-acorn26", "port-acorn32", "port-algor",
				"port-alpha", "port-amd64", "port-amiga", 
				"port-amigappc", "port-arc", "port-arm", 
				"port-atari", "port-bebox", "port-cats", 
				"port-cesfic", "port-cobalt", "port-dreamcast",
				"port-evbarm", "port-evbmips", "port-evbppc",
				"port-evbsh3", "port-evbsh5", "port-hp300",
				"port-hp700", "port-hpcarm", "port-hpcmips",
				"port-hpcsh", "port-hppa", "port-i386", "port-iyonix",
				"port-luna68k", "port-m68k", "port-mac68k", "port-macppc",
				"port-mips", "port-mipsco", "port-mmeye", "port-mvme68k",
				"port-mvmeppc", "port-netwinder", "port-news68k",
				"port-newsmips", "port-next68k", "port-ofppc", "port-pc532",
				"port-playstation2", "port-pmax", "port-pmppc", "port-powerpc",
				"port-prep", "port-sandpoint", "port-sbmips", "port-sgimips", 
				"port-sh3", "port-sh5", "port-shark", "port-sparc", "port-sparc64",
				"port-sun2", "port-sun3", "port-vax", "port-x68k", "port-xen", 
				"security", "standards", "toolchain", "xsrc", "y2k"
};

static char *pr_severities[]= { "non-critical", "serious", "critical" };
static char *pr_priorities[]= { "low", "medium", "high" };
static char *pr_classes[]= { "sw-bug", "doc-bug", "change-request", "support" };
static char default_to[]="gnats-bugs@gnats.netbsd.org";
static char default_rcpt[]="gnats bugs";
static char default_sub_id[]="net";

#endif

/* Show available categories */
void 
show_categories(void)
{
  int i;
  int len;
  int written;
  char my_fmt[16];

  printf("Known categories:\n");

  len = 0;

  for (i = 0; i < (sizeof(pr_categories)/sizeof(char *)); i++) {

    if (strlen(pr_categories[i]) > len ) len = strlen(pr_categories[i]);

  }

  snprintf(my_fmt, sizeof(my_fmt), "%%-%is ", len);

  written = 0;

  for (i = 0; i < (sizeof(pr_categories)/sizeof(char *)); i++) {

    written += printf(my_fmt, pr_categories[i]);

    if (written >= 70 ) {

      written = 0;
      printf("\n");

    }

  }

  printf("\n");

  exit(EXIT_SUCCESS);

}

/*
 * Fill a PR_DEFS structure with
 * PR info. Caller allocates (and frees)
 * the struct.
 */
int 
fill_prdefs(PR_DEFS *my_pr_defs)
{
  int i;

  i = sizeof(pr_categories)/sizeof(char *);
  my_pr_defs->pr_categories = malloc(i * sizeof(char *));

  for (i = 0; i < (sizeof(pr_categories)/sizeof(char *)); i++) {

    my_pr_defs->pr_categories[i] = malloc(strlen(pr_categories[i]) + 1);
    memset(my_pr_defs->pr_categories[i], 0, strlen(pr_categories[i]) + 1);
    strncpy(my_pr_defs->pr_categories[i], pr_categories[i], strlen(pr_categories[i]));
    my_pr_defs->cat_num++;

  }


  i = sizeof(pr_severities)/sizeof(char *);
  my_pr_defs->pr_severities = malloc(i * sizeof(char *));

  for (i = 0; i < (sizeof(pr_severities)/sizeof(char *)); i++) {

    my_pr_defs->pr_severities[i] = malloc(strlen(pr_severities[i]) + 1);
    memset(my_pr_defs->pr_severities[i], 0, strlen(pr_severities[i]) + 1);
    strncpy(my_pr_defs->pr_severities[i], pr_severities[i], strlen(pr_severities[i]));
    my_pr_defs->sev_num++;

  }

  i = sizeof(pr_priorities)/sizeof(char *);
  my_pr_defs->pr_priorities = malloc(i * sizeof(char *));

  for (i = 0; i < (sizeof(pr_priorities)/sizeof(char *)); i++) {

    my_pr_defs->pr_priorities[i] = malloc(strlen(pr_priorities[i]) + 1);
    memset(my_pr_defs->pr_priorities[i], 0, strlen(pr_priorities[i]) + 1);
    strncpy(my_pr_defs->pr_priorities[i], pr_priorities[i], strlen(pr_priorities[i]));
    my_pr_defs->pri_num++;

  }

  i = sizeof(pr_classes)/sizeof(char *);
  my_pr_defs->pr_classes = malloc(i * sizeof(char *));

  for (i = 0; i < (sizeof(pr_classes)/sizeof(char *)); i++) {

    my_pr_defs->pr_classes[i] = malloc(strlen(pr_classes[i]) + 1);
    memset(my_pr_defs->pr_classes[i], 0, strlen(pr_classes[i]) + 1);
    strncpy(my_pr_defs->pr_classes[i], pr_classes[i], strlen(pr_classes[i]));
    my_pr_defs->class_num++;

  }

  my_pr_defs->default_to = default_to;
  my_pr_defs->default_rcpt = default_rcpt;
  my_pr_defs->default_sub_id = default_sub_id;

  return 0;

}
