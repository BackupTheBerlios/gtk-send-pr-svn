# $Id$
# This is a BSD Makefile, not a GNUMakefile!
#
LOCALBASE?=/usr/local

CFLAGS+= -Wall -g -DGTK_DISABLE_DEPRECATED `pkg-config gtk+-2.0 --cflags` `libesmtp-config --cflags`

LDFLAGS+= `pkg-config gtk+-2.0 --libs` `libesmtp-config --libs`


.if defined(WITH_EFENCE)
LDFLAGS+= -L${LOCALBASE}/lib -lefence
.endif

OBJECTS= main.o gtk_ui.o smtp.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o gtk-send-pr $(OBJECTS)

clean:
	rm -f *.o gtk-send-pr gtk-send-pr.core
	
