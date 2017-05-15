
CC = gcc

CFLAGS += $(shell pkg-config --cflags json-c) -I"."
LDFLAGS += $(shell pkg-config --libs json-c) -L"."

src_c = wmappauth.c
src_h = wmappauth.h

default: wmappauth.o

sample_static: wmappauth.o
	$(CC) sample.c $(CFLAGS) $(LDFLAGS) -l:wmappauth.o -lcurl -o sample_static -l:credentials.o -l:objstring.o -l:terminput.o -l:objcurl.o -l:linkedlist.o

sample_dynamic: libwmappauth.so
	$(CC) sample.c $(CFLAGS) $(LDFLAGS) -lwmappauth -lcredentials -lobjstring -lterminput -lobjcurl -llinkedlist -lcurl -o sample_dynamic


#############
# wmappauth #
#############
wmappauth.o: $(src_c) credentials.o objstring.o terminput.o objcurl.o
	$(CC) -c wmappauth.c $(CFLAGS) $(LDFLAGS) -l:credentials.o -l:objstring.o -l:terminput.o -l:objcurl.o -l:linkedlist.o -lcurl -fpic

libwmappauth.so: $(src_c) libobjstring.so libcredentials.so libterminput.so libobjcurl.so wmappauth.o
	$(CC) -shared wmappauth.o -o libwmappauth.so $(CFLAGS) $(LDFLAGS) -lcredentials -lobjstring -lterminput -lobjcurl -llinkedlist -lcurl


##############
# LinkedList #
##############
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -I"." -c linkedlist.c -fpic

liblinkedlist.so: linkedlist.o
	$(CC) -I"." -shared linkedlist.o -o liblinkedlist.so


###########
# ObjCurl #
###########
objcurl.o: objcurl.c objcurl.h linkedlist.o objstring.o
	$(CC) -I"." -L"." -c objcurl.c -fpic -lcurl -l:linkedlist.o -l:objstring.o

libobjcurl.so: objcurl.o liblinkedlist.so libobjstring.so
	$(CC) -I"." -L"." -shared objcurl.o -o libobjcurl.so -llinkedlist -lobjstring


#############
# TermInput #
#############
terminput.o: terminput.h terminput.c
	$(CC) -I"." -c terminput.c -fpic

libterminput.so: terminput.o
	$(CC) -I"." -shared terminput.o -o libterminput.so


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
	$(RM) wmapp *.o *.so sample_dynamic sample_static
