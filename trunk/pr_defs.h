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
					
/* Since pkgsrc is multi-platform, we default to non-FreeBSD environments now */
#ifndef PR_DEFS_H
#define PR_DEFS_H

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

#define MAINT_CLASS "maintainer-update"
#define MAINT_CAT "ports"

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

#define MAINT_CLASS "change-request"
#define MAINT_CAT "pkg"

#endif

#endif
