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
#include <unistd.h>
#include <fcntl.h>

#include <gtk/gtk.h>

#include "../common/gtk-send-pr.h"
#include "gtk_ui.h"
#include "../common/pr_defs.h"

static void usage(void);
static void about(void);

int
main(int argc, char **argv)
{
  int ch;
  USER_OPTIONS *my_options;
  struct file_element *current;

  my_options = malloc(sizeof(USER_OPTIONS));
  memset(my_options, 0, sizeof(USER_OPTIONS));
  my_options->head = malloc(sizeof(struct file_element));
  memset(my_options->head, 0, sizeof(struct file_element));
  current = my_options->head;

  while ((ch = getopt(argc, argv, "a:mvhL")) != -1) {

    switch(ch) {

    case 'a':
      current->filename = malloc(FILENAME_MAX + 1);
      memset(current->filename, 0, FILENAME_MAX + 1);
      strncpy(current->filename, optarg, FILENAME_MAX);
      current->next = malloc(sizeof(struct file_element));
      memset(current->next, 0, sizeof(struct file_element));
      current = current->next;
      break;

    case 'v':
      about();
      break;

    case 'm':
      my_options->maint_mode = MAINT_YES;
      break;

    case 'L':
      show_categories();
      break;

    case '?':
    case 'h':
    default:
      usage();
      /* NOTREACHED */
    }

  }

  argc -= optind;
  argv += optind;

  gtk_init(&argc, &argv);

  create_gtk_ui(my_options);

  return(0);

}

static void
usage(void)
{
  printf("usage: gtk-send-pr [-a file] [-m] [-v] [-h] [-L]\n");
  printf("\t-a file\tinclude file in the Fix: section\n");
  printf("\t-m\tenable maintainer mode (see man page)\n");
  printf("\t-v\tshow version and exit\n");
  printf("\t-h\tshow this screen\n");
  printf("\t-L\tlist available categories and exit\n");
  exit(EXIT_SUCCESS);
}

static void
about(void)
{
  printf("gtk-send-pr " GSP_VERSION " " GSP_CODENAME "\n"
	 "Copyright (c) 2003 - 2006 Miguel Mendez <mmendez@energyhq.be>."
	 " All rights reserved.\n");
  exit(EXIT_SUCCESS);
}
