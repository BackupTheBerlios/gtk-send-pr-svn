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

OBJECTS= GTK/main.o GTK/gtk_ui.o common/user_settings.o BSD/smtp.o common/file.o common/uname_gather.o common/pr_defs.o
OBJ= main.o gtk_ui.o user_settings.o smtp.o file.o uname_gather.o pr_defs.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o gtk-send-pr $(OBJ)

install:
	${BSD_INSTALL_PROGRAM} gtk-send-pr ${PREFIX}/bin
	${BSD_INSTALL_MAN} common/gtk-send-pr.1 ${PREFIX}/man/man1

clean:
	rm -f *.o */*.c~ */*.h~ gtk-send-pr gtk-send-pr.core
