CC = gcc

CFLAGS += $(shell pkg-config --cflags json-c)
#CFLAGS += $(shell pkg-config --cflags wmappauth)
#CFLAGS += $(shell pkg-config --cflags terminput)
CFLAGS += $(shell pkg-config --cflags libcurl)
CFLAGS += -I"."
CFLAGS += -I"terminput" -I"credentials" -I"objstring" -I"objcurl" -I"linkedlist"

LDFLAGS += $(shell pkg-config --libs json-c)
#LDFLAGS += $(shell pkg-config --libs wmappauth)
#LDFLAGS += $(shell pkg-config --libs terminput)
LDFLAGS += $(shell pkg-config --libs libcurl)
LDFLAGS += -L"."
LDFLAGS += -L"terminput" -L"credentials" -L"objstring" -L"objcurl" -L"linkedlist"

src_c = wmappauth.c
src_h = wmappauth.h

default: static

sample_static: sample.c libwmappauth.a
	$(CC) $(CFLAGS) $(LDFLAGS) sample.c libwmappauth.a -o sample_static

sample_static_debug: sample.c libdwmappauth.a
	$(CC) $(CFLAGS) $(LDFLAGS) -g sample.c libdwmappauth.a -o sample_static_debug

sample_dynamic: libwmappauth.so credentials/libcredentials.so objstring/libobjstring.so objcurl/libobjcurl.so linkedlist/liblinkedlist.so terminput/libterminput.so
	$(CC) sample.c $(CFLAGS) $(LDFLAGS) -lwmappauth -lcredentials -lobjstring -lterminput -lobjcurl -llinkedlist -lcurl -o sample_dynamic -lm


#############
# wmappauth #
#############
static: libwmappauth.a

dynamic: libwmappauth.so

wmappauth.o: $(src_c)
	$(CC) -c $(src_c) $(CFLAGS) $(LDFLAGS) -fpic

libdwmappauth.a: $(src_c)
	$(CC) $(CFLAGS) $(LDFLAGS) -g -c $(src_c) -fpic
	cd credentials && $(CC) $(CFLAGS) $(LDFLAGS) -g -c credentials.c -fpic
	cd linkedlist && $(CC) $(CFLAGS) $(LDFLAGS) -g -c linkedlist.c -fpic
	cd objcurl && $(CC) $(CFLAGS) $(LDFLAGS) -I"../objstring" -I"../linkedlist" -g -c objcurl.c -fpic
	cd objstring && $(CC) $(CFLAGS) $(LDFLAGS) -g -c objstring.c -fpic
	cd terminput && $(CC) $(CFLAGS) $(LDFLAGS) -g -c terminput.c -fpic
	ar rcs libdwmappauth.a wmappauth.o credentials/credentials.o objstring/objstring.o objcurl/objcurl.o linkedlist/linkedlist.o terminput/terminput.o
	ranlib libdwmappauth.a

libwmappauth.a: wmappauth.o credentials/credentials.o objstring/objstring.o objcurl/objcurl.o linkedlist/linkedlist.o terminput/terminput.o
	ar rcs libwmappauth.a wmappauth.o credentials/credentials.o objstring/objstring.o objcurl/objcurl.o linkedlist/linkedlist.o terminput/terminput.o
	ranlib libwmappauth.a

libwmappauth.so: $(src_c) wmappauth.o
	$(CC) -shared wmappauth.o -o libwmappauth.so $(CFLAGS) $(LDFLAGS)


##############
# LinkedList #
##############
linkedlist/linkedlist.o: linkedlist/Makefile
	cd linkedlist && $(MAKE) linkedlist.o

linkedlist/liblinkedlist.so: linkedlist/Makefile
	cd linkedlist && $(MAKE) liblinkedlist.so


###########
# ObjCurl #
###########
objcurl/objcurl.o: objcurl/Makefile
	cd objcurl && $(MAKE) objcurl.o

objcurl/libobjcurl.so: objcurl/Makefile
	cd objcurl && $(MAKE) libobjcurl.so


#############
# TermInput #
#############
terminput/terminput.o: terminput/Makefile
	cd terminput && $(MAKE) terminput.o

terminput/libterminput.so: terminput/Makefile
	cd terminput && $(MAKE) libterminput.so


###############
# Credentials #
###############
credentials/credentials.o: credentials/Makefile
	cd credentials && $(MAKE) credentials.o

credentials/libcredentials.so: credentials/Makefile
	cd credentials && $(MAKE) libcredentials.so


#############
# ObjString #
#############
objstring/objstring.o: objstring/Makefile
	cd objstring && $(MAKE) objstring.o

objstring/libobjstring.so: objstring/Makefile
	cd objstring && $(MAKE) libobjstring.so



prepare:
	sudo dnf install libcurl-devel

clean:
	$(RM) wmapp *.o *.so *.a sample_dynamic sample_static sample_static_debug
	cd credentials && $(MAKE) clean
	cd objcurl && $(MAKE) clean
	cd linkedlist && $(MAKE) clean
	cd terminput && $(MAKE) clean
	cd objstring && $(MAKE) clean
