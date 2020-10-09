plugin_LTLIBRARIES = liblmon.la
plugindir = $(libdir)/xfce4/panel/plugins
liblmon_la_CFLAGS = -Iinclude $(LIBXFCE4PANEL_CFLAGS) $(PLATFORM_CFLAGS)
liblmon_la_LDFLAGS = -avoid-version -shared $(PLATFORM_LDFLAGS)
liblmon_la_SOURCES = \
	src/lmon.c src/load.c \
	include/lmon.h include/lmon_resource.h
liblmon_la_LIBADD = $(LIBXFCE4PANEL_LIBS) lmon_resource.o

# Possibly hacky. sorry
lmon_resource.o : data/lmon.ui data/lmon.css
	ld -r -b binary $^ -o $@

desktop_DATA = data/lmon.desktop
desktopdir = $(datadir)/xfce4/panel/plugins

dist_doc_DATA = README.md LICENSE.md

EXTRA_DIST = data/lmon.desktop data/lmon.ui data/lmon.css
