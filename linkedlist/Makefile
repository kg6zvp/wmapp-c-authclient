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
static: liblinkedlist.a

#make it clear that this is the dynamically linked version
dynamic: liblinkedlist.so

##############
# LinkedList #
##############
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -I"." -c linkedlist.c -fpic

liblinkedlist.a: linkedlist.o
	ar rcs liblinkedlist.a linkedlist.o
	ranlib liblinkedlist.a

liblinkedlist.so: linkedlist.o
	$(CC) -I"." -shared linkedlist.o -o liblinkedlist.so

install: liblinkedlist.a liblinkedlist.so linkedlist.h mc_linkedlist.pc
	mkdir -p "$(lib64dir)"
	mkdir -p "$(includedir)"
	cp linkedlist.h "$(includedir)/"
	cp liblinkedlist.a "$(lib64dir)/"
	cp liblinkedlist.so "$(lib64dir)/"
	cp mc_linkedlist.pc "/usr/lib64/pkgconfig/"

uninstall:
	$(RM) "$(lib64dir)/liblinkedlist.a" "$(lib64dir)/liblinkedlist.so" "$(includedir)/linkedlist.h" "/usr/lib64/pkgconfig/mc_linkedlist.pc"

clean:
	$(RM) *.o *.so *.a
