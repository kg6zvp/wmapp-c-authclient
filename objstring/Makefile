CC=gcc

lib32pfx="/usr/lib"
lib64pfx="/usr/lib64"
includepfx="/usr/include"
libSuffix="mccollum"

lib64dir="$(lib64pfx)/$(libSuffix)"
includedir="$(includepfx)/$(libSuffix)"

# Default to dynamically linked library
default: dynamic

all: dynamic static

#make it clear that it's the static version
static: libobjstring.a

#make it clear that this is the dynamically linked version
dynamic: libobjstring.so

#############
# ObjString #
#############
objstring.o: objstring.h objstring.c
	$(CC) -I"." -c objstring.c -fpic

libobjstring.a: objstring.o
	ar rcs libobjstring.a objstring.o
	ranlib libobjstring.a

libobjstring.so: objstring.o
	$(CC) -I"." -shared objstring.o -o libobjstring.so

install: libobjstring.a libobjstring.so objstring.h mc_objstring.pc
	mkdir -p "$(lib64dir)"
	mkdir -p "$(includedir)"
	cp objstring.h "$(includedir)/"
	cp libobjstring.a "$(lib64dir)/"
	cp libobjstring.so "$(lib64dir)/"
	cp mc_objstring.pc "/usr/lib64/pkgconfig/"

uninstall:
	$(RM) "$(lib64dir)/libobjstring.a" "$(lib64dir)/libobjstring.so" "$(includedir)/objstring.h" "/usr/lib64/pkgconfig/mc_objstring.pc"

clean:
	$(RM) *.o *.so *.a
