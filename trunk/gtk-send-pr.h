/*
Copyright (c) 2003, Miguel Mendez. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer. 
	* Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation 
	and/or other materials provided with the distribution. 
	* Neither the name of Miguel Mendez nor the names of his contributors
	may be used to endorse or promote products derived from this software 
	without specific prior written permission.

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

#define GSP_VERSION "0.1"

typedef struct user_prfile {

	char name[255];
	char email[255];
	char org[255];
	
} USER_PROFILE;


						
#if defined(__NetBSD__)
char *pr_categories[]= { "bin", "install", "kern", "lib", "misc", "pkg", \
					"port-acorn26", "port-acorn32", "port-algor", \
					"port-alpha", "port-amiga", "port-arc", \
					"port-arm"," port-arm32", "port-atari", \
					"port-bebox", "port-cats", "port-cobalt", \
					"port-dreamcast", "port-evbmips", "port-evbsh5", \
					"port-hp300", "port-hp700", "port-hpcmips", \
					"port-hpcsh", "port-hppa", "port-i386", \
					"port-luna68k", "port-m68k", "port-mac68k", \
					"port-macppc", "port-mips", "port-mvme68k", \
					"port-mvmeppc", "port-news68k", "port-newsmips", \
					"port-next68k", "port-ofppc", "port-pc532", \
					"port-playstation2", "port-pmax", "port-powerpc", \
					"port-prep", "port-sbmips", "port-sgimips", \
					"port-sh3", "port-sh5", "port-shark", \
					"port-sparc", "port-sparc64", "port-sun2", \
					"port-sun3", "port-vax", "port-x68k", \
					"port-x86_64", "security", "standards", \
					"toolchain", "xsrc", "y2k"};

char *pr_severities[]= { "non-critical", "serious", "critical" };
char *pr_priorities[]= { "low", "medium", "high" };
char *pr_classes[]= { "sw-bug", "doc-bug", "change-request", "support" };
char default_to[]="gnats-bugs@gnats.netbsd.org";
char default_sub_id[]="net";

#elif defined(__FreeBSD__)


#endif

#define KNOB_IS_YES 1
#define KNOB_IS_NO 0
#define IS_KNOB 1
#define NOT_KNOB 0
#define MODIFIED_NO 0
#define MODIFIED_YES 1


