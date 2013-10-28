VERSION = XFireworks-1.3
PKGNAME = xfireworks-1.3
PREFIX = /usr/X11R6
X11BASE = /usr/X11R6

BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/etc
MANDIR = $(PREFIX)/man/man1

XINCLUDEDIR = $(X11BASE)/include
XLIBDIR = $(X11BASE)/lib

CFLAGS = -O
LFLAGS = -lm -lX11
CC = cc
CP = cp
RM = rm -f
MKDIR = mkdir -p
RMDIR = rmdir
CHMOD = chmod

XFIREWORKS_CONF_DIRECTRY = $(LIBDIR)

OBJS = main.o AfterImage.o Disp.o ColorGC.o Obj.o Stream.o Piece.o XFireworks.o Calculator.o etc.o arguments.o

all :		xfireworks manual

xfireworks :	$(OBJS)
		$(CC) -o xfireworks $(OBJS) -L$(XLIBDIR) $(CFLAGS) $(LFLAGS)

main.o :	main.c
		$(CC) -c main.c $(CFLAGS) \
		-DVERSION=\"$(VERSION)\" \
		-DXFIREWORKS_CONF_DIRECTRY=\"$(XFIREWORKS_CONF_DIRECTRY)\"

AfterImage.o :	AfterImage.c
		$(CC) -c AfterImage.c -I$(XINCLUDEDIR) $(CFLAGS)

Disp.o :	Disp.c
		$(CC) -c Disp.c -I$(XINCLUDEDIR) $(CFLAGS)

ColorGC.o :	ColorGC.c
		$(CC) -c ColorGC.c -I$(XINCLUDEDIR) $(CFLAGS)

Obj.o :		Obj.c
		$(CC) -c Obj.c $(CFLAGS)

Stream.o :	Stream.c
		$(CC) -c Stream.c $(CFLAGS)

Calculator.o :	Calculator.c
		$(CC) -c Calculator.c $(CFLAGS)

Piece.o :	Piece.c
		$(CC) -c Piece.c -I$(XINCLUDEDIR) $(CFLAGS)

XFireworks.o :	XFireworks.c xfireworks_conf.h
		$(CC) -c XFireworks.c -I$(XINCLUDEDIR) $(CFLAGS)

xfireworks_conf.h :	xfireworks.conf mkconf
		cat xfireworks.conf | ./mkconf > xfireworks_conf.h

mkconf :	mkconf.c
		$(CC) mkconf.c -o mkconf

etc.o :		etc.c
		$(CC) -c etc.c $(CFLAGS)

arguments.o :	arguments.c
		$(CC) -c arguments.c $(CFLAGS)

manual :	xfireworks.1.gz

xfireworks.1.gz :	xfireworks.1
		cat xfireworks.1 | gzip > xfireworks.1.gz

clean :
		$(RM) *.o xfireworks xfireworks_conf.h mkconf xfireworks.1.gz

install :	install-bin install-lib install-man

install-bin :	xfireworks
		$(MKDIR) $(BINDIR)
		$(CP) xfireworks $(BINDIR)
		$(CHMOD) 0755 $(BINDIR)/xfireworks

install-lib :
		$(MKDIR) $(LIBDIR)
		$(CP) xfireworks.conf $(LIBDIR)
		$(CHMOD) 0644 $(LIBDIR)/xfireworks.conf

install-man :	manual
		$(MKDIR) $(MANDIR)
		$(CP) xfireworks.1.gz $(MANDIR)
		$(CHMOD) 0444 $(MANDIR)/xfireworks.1.gz

uninstall :	uninstall-bin uninstall-lib uninstall-man

uninstall-bin :
		$(RM) $(BINDIR)/xfireworks

uninstall-lib :
		$(RM) $(LIBDIR)/xfireworks.conf

uninstall-man :
		$(RM) $(MANDIR)/xfireworks.1.gz

package :
		$(MKDIR) $(PKGNAME)
		$(RM) $(PKGNAME)/*
		$(CP) AUTHORS COPYING COPYRIGHT ChangeLog HISTORY INSTALL \
			NEWS README OMAKE.jpn Makefile xfireworks.conf \
			xfireworks.1 *.h *.c $(PKGNAME)
		$(RM) $(PKGNAME)/xfireworks_conf.h
		tar cvzf $(PKGNAME).tar.gz $(PKGNAME)

# End of Makefile.
