# $Id$
# This is a BSD Makefile, not a GNUMakefile!
#
LOCALBASE?=/usr/local

GTK_CFLAGS!=pkg-config gtk+-2.0 --cflags
ESMTP_CFLAGS!=libesmtp-config --cflags
CFLAGS+= -Wall -g -DGTK_DISABLE_DEPRECATED ${GTK_CFLAGS} ${ESMTP_CFLAGS}

GTK_LDFLAGS!=pkg-config gtk+-2.0 --libs
ESMTP_LDFLAGS!=libesmtp-config --libs
LDFLAGS+= ${GTK_LDFLAGS} ${ESMTP_LDFLAGS}

.if defined(WITH_EFENCE)
LDFLAGS+= -L${LOCALBASE}/lib -lefence
.endif

OBJECTS= main.o gtk_ui.o user_settings.o smtp.o file.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o gtk-send-pr $(OBJECTS)

clean:
	rm -f *.o gtk-send-pr gtk-send-pr.core

