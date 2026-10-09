DIST_SUBDIRS = base gtk data

if AIK_FE_GTK
GTKSRCDIR = gtk
else
GTKSRCDIR = 
endif

SUBDIRS = base $(GTKSRCDIR) data

EXTRA_DIST =	\
	autogen.sh			\
	ac-helpers/pkg-config/pkg.m4	\
	aiksaurus.spec.in		\
	aiksaurus.spec
