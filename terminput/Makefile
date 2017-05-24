CC=gcc

lib32pfx="/usr/lib"
lib64pfx="/usr/lib64"
includepfx="/usr/include"
libSuffix="mccollum"

lib64dir="$(lib64pfx)/$(libSuffix)"
includedir="$(includepfx)/$(libSuffix)"

# Default to dynamically linked library
default: dynamic

all: static dynamic

#make it clear that it's the static version
static: libterminput.a

#make it clear that this is the dynamically linked version
dynamic: libterminput.so

#############
# TermInput #
#############
terminput.o: terminput.h terminput.c
	$(CC) -I"." -c terminput.c -fpic

libterminput.a: terminput.o
	ar rcs libterminput.a terminput.o
	ranlib libterminput.a

libterminput.so: terminput.o
	$(CC) -I"." -shared terminput.o -o libterminput.so

install: libterminput.a libterminput.so terminput.h mc_terminput.pc
	mkdir -p "$(lib64dir)"
	mkdir -p "$(includedir)"
	cp terminput.h "$(includedir)/"
	cp libterminput.a "$(lib64dir)/"
	cp libterminput.so "$(lib64dir)/"
	cp mc_terminput.pc "/usr/lib64/pkgconfig/"

uninstall:
	$(RM) "$(lib64dir)/libterminput.a" "$(lib64dir)/libterminput.so" "$(includedir)/terminput.h" "/usr/lib64/pkgconfig/mc_terminput.pc"

clean:
	$(RM) *.o *.so

