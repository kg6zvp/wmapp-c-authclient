CC = gcc

CFLAGS += $(shell pkg-config --cflags json-c)
CFLAGS += $(shell pkg-config --cflags mc_objcurl mc_credentials mc_terminput mc_linkedlist mc_objstring)
CFLAGS += -I"."

LDFLAGS += $(shell pkg-config --libs json-c)
LDFLAGS += $(shell pkg-config --libs mc_objcurl mc_credentials mc_terminput mc_linkedlist mc_objstring)


lib32pfx="/usr/lib"
lib64pfx="/usr/lib64"
includepfx="/usr/include"
libSuffix="mccollum"

lib64dir="$(lib64pfx)/$(libSuffix)"
includedir="$(includepfx)/$(libSuffix)"

src_c = wmappauth.c
src_h = wmappauth.h

default: static

all: dynamic static

#############
# wmappauth #
#############
static: libwmappauth.a

dynamic: libwmappauth.so

wmappauth.o: $(src_c) $(src_h)
	$(CC) -c $(src_c) $(CFLAGS) $(LDFLAGS) -fpic

libwmappauth.a: wmappauth.o
	ar rcs libwmappauth.a wmappauth.o
	ranlib libwmappauth.a

libwmappauth.so: wmappauth.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared wmappauth.o -o libwmappauth.so

prepare:
	sudo dnf install libcurl-devel json-c-devel

install: libwmappauth.a wmappauth.o libwmappauth.so wmappauth.h mc_wmappauth.pc mc_wmappauth_static.pc
	mkdir -p "$(lib64dir)"
	mkdir -p "$(includedir)"
	cp wmappauth.h "$(includedir)/"
	cp libwmappauth.a "$(lib64dir)/"
	cp wmappauth.o "$(lib64dir)/"
	cp libwmappauth.so "$(lib64dir)/"
	cp mc_wmappauth.pc "/usr/lib64/pkgconfig/"
	cp mc_wmappauth_static.pc "/usr/lib64/pkgconfig/"

uninstall:
	$(RM) "$(lib64dir)/libwmappauth.a" "$(lib64dir)/wmappauth.o" "$(lib64dir)/libwmappauth.so" "$(includedir)/wmappauth.h" "/usr/lib64/pkgconfig/mc_wmappauth.pc" "/usr/lib64/pkgconfig/mc_wmappauth_static.pc"

clean:
	$(RM) *.o *.so *.a
