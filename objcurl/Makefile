CC=gcc

CFLAGS += $(shell pkg-config --cflags mc_linkedlist mc_objstring)
LDFLAGS += $(shell pkg-config --libs mc_linkedlist mc_objstring)

#-I"." -I"../objstring" -I"../linkedlist"
#-L"." -L"../objstring" -L"../linkedlist"

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
static: libobjcurl.a

#make it clear that this is the dynamically linked version
dynamic: libobjcurl.so

###########
# ObjCurl #
###########
objcurl.o: objcurl.c objcurl.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I"." -c objcurl.c -fpic

libobjcurl.a: objcurl.o
	ar rcs libobjcurl.a objcurl.o
	ranlib libobjcurl.a

libobjcurl.so: objcurl.o
	$(CC) $(CFLAGS) $(LDFLAGS) -I"." -shared objcurl.o -o libobjcurl.so

install: libobjcurl.a libobjcurl.so objcurl.h mc_objcurl.pc
	mkdir -p "$(lib64dir)"
	mkdir -p "$(includedir)"
	cp objcurl.h "$(includedir)/"
	cp libobjcurl.a "$(lib64dir)/"
	cp libobjcurl.so "$(lib64dir)/"
	cp mc_objcurl.pc "/usr/lib64/pkgconfig/"

uninstall:
	$(RM) "$(lib64dir)/libobjcurl.a" "$(lib64dir)/libobjcurl.so" "$(includedir)/objcurl.h" "/usr/lib64/pkgconfig/mc_objcurl.pc"

clean:
	$(RM) *.so *.o *.a

