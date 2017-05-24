CC = gcc

# Default to dynamically linked library
default: dynamic

all: dynamic static

dynamic: libcredentials.so

static: libcredentials.a

###############
# Credentials #
###############
credentials.o: credentials.c credentials.h
	$(CC) -I"." -c credentials.c -fpic

libcredentials.a: credentials.o
	ar rcs libcredentials.a credentials.o
	ranlib libcredentials.a

libcredentials.so: credentials.o
	$(CC) -I"." -shared credentials.o -o libcredentials.so

install: libcredentials.a libcredentials.so credentials.o credentials.h mc_credentials.pc
	mkdir -p "/usr/lib64/mccollum"
	mkdir -p "/usr/include/mccollum"
	cp credentials.h /usr/include/mccollum/
	cp libcredentials.a /usr/lib64/mccollum/
	cp libcredentials.so /usr/lib64/mccollum/
	cp mc_credentials.pc /usr/lib64/pkgconfig/

uninstall:
	$(RM) /usr/lib64/mccollum/libcredentials.a /usr/lib64/mccollum/libcredentials.so /usr/lib64/pkgconfig/mc_credentials.pc /usr/include/mccollum/credentials.h

clean:
	$(RM) *.o *.so *.a
