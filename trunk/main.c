/*
  Copyright (c) 2003-2005, Miguel Mendez. All rights reserved.

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

#include "gtk-send-pr.h"
#include "gtk_ui.h"


static void usage(void);
static void about(void);

int
main(int argc, char **argv)
{
  int ch;
  char included_file[FILENAME_MAX];
  int maintainer = MAINT_NO;
  int use_file = 0;

  memset(included_file, 0, sizeof(included_file));

  while ((ch = getopt(argc, argv, "a:mvhL")) != -1) {

    switch(ch) {

    case 'a':
      strncpy(included_file, optarg, sizeof(included_file) - 1);
      use_file = 1;
      break;

    case 'v':
      about();
      break;

    case 'm':
      maintainer = MAINT_YES;
      break;

    case 'L':
      show_categories();
      break;
    case '?':
    case 'h':
    default:
      usage();

    }

  }

  argc -= optind;
  argv += optind;

  gtk_init(&argc, &argv);

  if(use_file == 1) {

    create_gtk_ui(included_file, maintainer);

  } else {

    create_gtk_ui(NULL, maintainer);

  }

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
	 "Copyright (c) 2003-2005, Miguel Mendez."
	 " All rights reserved.\n");
  exit(EXIT_SUCCESS);
}
