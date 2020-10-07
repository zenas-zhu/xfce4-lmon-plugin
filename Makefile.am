plugin_LTLIBRARIES = liblmon.la
plugindir = $(libdir)/xfce4/panel/plugins
liblmon_la_CFLAGS = $(LIBXFCE4PANEL_CFLAGS) -Iinclude
liblmon_la_LDFLAGS = $(LIBXFCE4PANEL_LIBS) -avoid-version -shared
liblmon_la_SOURCES = src/lmon.c src/load.c
liblmon_la_LIBADD = lmon_resource.o

# Possibly hacky. sorry
lmon_resource.o : data/lmon.ui data/lmon.css
	ld -r -b binary $^ -o $@

desktop_DATA = data/lmon.desktop
desktopdir = $(datadir)/xfce4/panel/plugins

dist_doc_DATA = README.md LICENSE.md
