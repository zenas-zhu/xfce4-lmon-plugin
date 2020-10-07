CC=gcc
CFLAGS=-Wall -Wextra -O3

XFCE4_PANEL_2_0_INCS=$$(pkg-config --cflags libxfce4panel-2.0)
XFCE4_PANEL_2_0_LIBS=$$(pkg-config --libs libxfce4panel-2.0)

LIBDEST=/usr/lib/xfce4/panel/plugins
SHAREDEST=/usr/share/xfce4/panel/plugins

HEADERS=include/lmon.h include/lmon_resource.h
OBJECTS=obj/lmon.o obj/load.o obj/lmon.resource.o
FINAL=obj/liblmon.so

.PHONY : all install uninstall clean

all : obj obj/liblmon.so

obj :
	mkdir -p $@

obj/liblmon.so : $(OBJECTS)
	$(CC) $(CFLAGS) -shared -fPIC $^ $(XFCE4_PANEL_2_0_LIBS) -o $@

obj/lmon.o : src/lmon.c $(HEADERS)
	$(CC) $(CFLAGS) -Iinclude -fPIC $(XFCE4_PANEL_2_0_INCS) -c $< -o $@

obj/load.o : src/load.c $(HEADERS)
	$(CC) $(CFLAGS) -Iinclude -fPIC $(XFCE4_PANEL_2_0_INCS) -c $< -o $@

obj/lmon.resource.o : data/lmon.ui data/lmon.css
	ld -r -b binary $^ -o $@

install : obj/liblmon.so
	install -m755 -t $(LIBDEST) obj/liblmon.so
	install -m644 -t $(SHAREDEST) data/lmon.desktop

uninstall :
	rm -f $(LIBDEST)/liblmon.so $(SHAREDEST)/lmon.desktop

clean :
	rm -f obj/liblmon.so $(OBJECTS)
