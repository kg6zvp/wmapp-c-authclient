
CC = gcc

LDFLAGS = -lcurl

CFLAGS += $(shell pkg-config --cflags json-c) -I"."
LDFLAGS += $(shell pkg-config --libs json-c) -L"."

src_c = wmappauth.c
src_h = wmappauth.h

default: static

static: $(src_c) credentials.o objstring.o
	$(CC) wmappauth.c -o wmapp $(CFLAGS) $(LDFLAGS) -l:credentials.o -l:objstring.o

dynamic: $(src_c) objstring.so credentials.so
	echo "NOT IMPLEMENTED"


###############
# Credentials #
###############
credentials.o: credentials.c credentials.h
	$(CC) -I"." -c credentials.c -fpic

libcredentials.so: credentials.o
	$(CC) -I"." -shared credentials.o -o libcredentials.so


#############
# ObjString #
#############
objstring.o: objstring.h objstring.c
	$(CC) -I"." -c objstring.c -fpic

libobjstring.so: objstring.o
	$(CC) -I"." -shared objstring.o -o libobjstring.so



prepare:
	sudo dnf install libcurl-devel

clean:
	$(RM) wmapp credentials.o objstring.o libcredentials.so libobjstring.so
