/*
  Copyright (c) 2003, 2004, 2005 Miguel Mendez <flynn@energyhq.es.eu.org>

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
#include <libgen.h>

#include "file.h"

/*
 * This function loads a file into a char * buffer, 
 * and puts a header and a footnote.
 * NOTE: Caller must free the buffer.
 */
char * 
load_file(char *filename)
{
  int fd;
  int i;
  char *fix_buffer;
  char file_head[1024];
  char file_foot[1024];

  fd = open(filename, O_RDONLY, 0);
  if (fd != -1) {

    i = lseek(fd,0,SEEK_END);
    fix_buffer = malloc(i + 2048);
    snprintf(file_head, 1024, "--- %s begins here ---\n", basename(filename));
    snprintf(file_foot, 1024, "--- %s ends here ---\n", basename(filename));
    lseek(fd, 0, SEEK_SET);
    strncpy(fix_buffer, file_head, 1024);
    read(fd, fix_buffer+strlen(file_head), i);

    if (*(fix_buffer + strlen(file_head) + i - 1) != '\n') {
      strcpy(fix_buffer+strlen(file_head)+i, "\n");
      i++;
    }
    strncpy(fix_buffer+strlen(file_head) + i, file_foot, 1024);
    close(fd);
    return fix_buffer;

  } else {

    return NULL;

  }

}
