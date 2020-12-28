# Makefile.am file for gtk-knocker
# Edited by Gabriele Giorgetti <g.gabriele79@genie.it>

## Process this file with automake to produce Makefile.in

## Any directories that are built and installed should go here.
SUBDIRS = src docs

## Any directories to be a part of the distribution should be listed
## here, as well as have a Makefile generated at the end of configure.in
DIST_SUBDIRS = $(SUBDIRS) docs

SUBDIRS = src


# All the rest of the distributed files
EXTRA_DIST = \
	AUTHORS 	\
	BUGS            \
	COPYING         \
        INSTALL         \
        README		\
	TO-DO    	\
	autogen.sh	\
	ChangeLog	\
	gtk-knocker.glade



install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

