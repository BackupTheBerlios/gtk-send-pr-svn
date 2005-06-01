# $Id$
# This is a BSD Makefile, not a GNUMakefile!
#
LOCALBASE?=/usr/local

GTK_CFLAGS!=pkg-config gtk+-2.0 --cflags
ESMTP_CFLAGS!=libesmtp-config --cflags
CFLAGS+= -Wall -g ${GTK_CFLAGS} ${ESMTP_CFLAGS}

GTK_LDFLAGS!=pkg-config gtk+-2.0 --libs
ESMTP_LDFLAGS!=libesmtp-config --libs
LDFLAGS+= ${GTK_LDFLAGS} ${ESMTP_LDFLAGS}

OBJECTS= main.o gtk_ui.o user_settings.o smtp.o file.o uname_gather.o pr_defs.o
HEADERS= file.h gtk_ui.h smtp.h gtk-send-pr.h pr_defs.h user_settings.h uname_gather.h

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o gtk-send-pr $(OBJECTS)

install:
	${BSD_INSTALL_PROGRAM} gtk-send-pr ${PREFIX}/bin
	${BSD_INSTALL_MAN} gtk-send-pr.1 ${PREFIX}/man/man1

clean:
	rm -f *.o *.c~ *.h~ gtk-send-pr gtk-send-pr.core
