/*
Copyright (c) 2003, Miguel Mendez. All rights reserved.

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
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <gtk/gtk.h>
#include "gtk_ui.h"


void usage(void);

	char *included_file=NULL;
	
int
main(int argc, char **argv)
{
	int ch;
	
	while((ch = getopt(argc, argv, "avh")) != -1) {
		
		switch(ch) {
			case 'a':
					if(argc>2) {
						included_file=argv[optind];
					} else {
						usage();
					}
					break;
			case 'v':
					printf("gtk-send-pr 0.2 \"Southern Sun\"\n"
					"Copyright (c) 2003, Miguel Mendez."
					" All rights reserved.\n");
					exit(EXIT_SUCCESS);
			case 'h':
			default:
					usage();
		}
	argc -= optind;
	argv += optind;

	}
		
	gtk_init (&argc, &argv);
	
	create_gtk_ui();
	return(0);
}

void
usage(void)
{
	printf("usage: gtk-send-pr [-a file] [-v] [-h]\n");
	printf("\t-a file\tinclude file in the Fix: section\n");
	printf("\t-v\tshow version and exit\n");
	printf("\t-h\tshow this screen\n");
	exit(EXIT_SUCCESS);
}
