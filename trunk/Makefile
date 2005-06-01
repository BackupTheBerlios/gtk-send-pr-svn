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
LDFLAGS+= `pkg-config gtk+-2.0 --libs` -lncurses
.endif

.if defined(WITH_EFENCE)
LDFLAGS+= -L${LOCALBASE}/lib -lefence
.endif

.if !defined (NO_GUI)
OBJECTS= main.o gtk_ui.o
.else 
OBJECTS= main.o
.endif

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o gtk-send-pr $(OBJECTS)

clean:
	rm -f *.o gtk-send-pr gtk-send-pr.core
	
