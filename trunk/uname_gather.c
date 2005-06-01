/*
  Copyright (c) 2004, Miguel Mendez. All rights reserved.

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
#include <sys/utsname.h>

#if defined (__FreeBSD__) || defined (__DragonFly__)
#include <sys/types.h>
#elif defined (__NetBSD__)
#include <sys/param.h>
#endif

#if defined (__FreeBSD__) || defined (__DragonFly__) || defined (__NetBSD__)
#include <sys/sysctl.h>
#else
#define USE_OLD_UNAME_METHOD
#endif

#include "uname_gather.h"

/* Stores uname info in two char arrays.
 * The first one corresponds to the uname -srm
 * The second one has the output from uname -a,
 * gathered from the sysctl info. Using the uname
 * method truncates part of the output in DFly.
 */
int 
uname_gather(char *uname_srm,char *uname_snrvm)
{

  int i;

#if !defined(USE_OLD_UNAME_METHOD)
  int mib[2];
  size_t len;
  char *output;
#endif

  struct utsname my_uname;

  i=uname(&my_uname);
  if(i==-1) {
    perror("uname()");
    exit(EXIT_FAILURE);
  }

  snprintf(uname_srm,255,"%s %s %s",my_uname.sysname,
	   my_uname.release,
	   my_uname.machine);


  /* 
   * Use this method for people using pkgsrc in e.g.
   * GNU/Linux/GNU, which uses its own proprietary and
   * incompatible sysctl interface.
   */

#if defined (USE_OLD_UNAME_METHOD)

  snprintf(uname_snrvm,1023,"System: %s %s %s %s %s",
	   my_uname.sysname,
	   my_uname.nodename,
	   my_uname.release,
	   my_uname.version,
	   my_uname.machine);
#else

  mib[0] = CTL_KERN;
  mib[1] = KERN_VERSION;
  sysctl(mib, 2, NULL, &len, NULL, 0);
  output = malloc(len);
  sysctl(mib, 2, output, &len, NULL, 0);

  snprintf(uname_snrvm,1023,"System: %s",output);

  free(output);

#endif

  return 0;

}

