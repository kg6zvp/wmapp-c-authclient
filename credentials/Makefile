CC = gcc

# Default to dynamically linked library
default: dynamic

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
	mkdir -p "/usr/lib64/mccollum/credentials/"
	cp libcredentials.a /usr/lib64/mccollum/credentials/
	cp libcredentials.so /usr/lib64/mccollum/credentials/
	cp mc_credentials.pc /usr/lib64/pkgconfig/

uninstall: /usr/lib64/mccollum/credentials/libcredentials.a /usr/lib64/mccollum/credentials/libcredentials.so /usr/lib64/pkgconfig/mc_credentials.pc
	$(RM) /usr/lib64/mccollum/credentials/libcredentials.a /usr/lib64/mccollum/credentials/libcredentials.so /usr/lib64/pkgconfig/mc_credentials.pc

clean:
	$(RM) *.o *.so *.a
