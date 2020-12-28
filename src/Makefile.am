## Process this file with automake to produce Makefile.in

INCLUDES = \
	@GTK_CFLAGS@

bin_PROGRAMS = gtk-knocker

gtk_knocker_SOURCES = \
	gtk-knocker.c global.h		\
	gtk_support.c gtk_support.h 	\
	gtk_interface.c gtk_interface.h \
	gtk_callbacks.c gtk_callbacks.h \
	knocker_core.c knocker_core.h	\
	knocker_services.c 		\
	knocker_services.h 		\
	knocker_args.c knocker_args.h	\
	knocker_user.c knocker_user.h   \
	knocker_log.c knocker_log.h	\
	knocker_time.c knocker_time.h	\
	license.h



gtk_knocker_LDADD = @GTK_LIBS@

