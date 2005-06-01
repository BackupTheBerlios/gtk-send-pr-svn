# $Id$
# This is a BSD Makefile, not a GNUMakefile!
#
LOCALBASE?=/usr/local


.if !defined(NO_GUI)
CFLAGS+= -Wall -g -DGTK_DISABLE_DEPRECATED `pkg-config gtk+-2.0 --cflags`
.else 
CFLAGS+= -Wall -DNO_GUI
.endif


.if !defined(NO_GUI)
.if !defined(WITH_CDK)
LDFLAGS+= `pkg-config gtk+-2.0 --libs` -ldialog -lncurses
.else
LDFLAGS+= `pkg-config gtk+-2.0 --libs` -L${LOCALBASE}/lib -lcdk -lncurses
.endif
.else
.if !defined(WITH_CDK)
LDFLAGS+= -ldialog -lncurses
.else
LDFLAGS+= -L${LOCALBASE}/lib -lncurses -lcdk
.endif
.endif

.if defined(WITH_EFENCE)
LDFLAGS+= -L${LOCALBASE}/lib -lefence
.endif

.if !defined (NO_GUI)
OBJECTS= ${SYSNAME}_parser.o main.o gtk_ui.o
.else 
OBJECTS= ${SYSNAME}_parser.o main.o
.endif

.if defined(WITH_CDK)
OBJECTS+= cdk_ui.o
.else
OBJECTS+= ncurses_ui.o
.endif

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o thefish $(OBJECTS)

clean:
	rm -f *.o thefish thefish.core
	
