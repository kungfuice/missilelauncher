## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = missilelauncher

missilelauncher_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	missile.c	missile.h \
	callbacks.c callbacks.h

missilelauncher_LDADD = @PACKAGE_LIBS@ $(INTLLIBS) \
												-lusb
