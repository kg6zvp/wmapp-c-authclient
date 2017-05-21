CC=gcc

CFLAGS += $(shell pkg-config --cflags linkedlist objstring) -I"." -I"../objstring" -I"../linkedlist"
LDFLAGS += $(shell pkg-config --libs linkedlist objstring) -L"." -L"../objstring" -L"../linkedlist"

# Default to dynamically linked library
default: dynamic

#make it clear that it's the static version
static: libobjcurl.a

#make it clear that this is the dynamically linked version
dynamic: libobjcurl.so

###########
# ObjCurl #
###########
objcurl.o: objcurl.c objcurl.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c objcurl.c -fpic

libobjcurl.a: objcurl.o ../objstring/objstring.o ../linkedlist/linkedlist.o
	ar rcs libobjcurl.a objcurl.o ../objstring/objstring.o ../linkedlist/linkedlist.o
	ranlib libobjcurl.a

libobjcurl.so: objcurl.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared objcurl.o -o libobjcurl.so

clean:
	$(RM) *.so *.o *.a

