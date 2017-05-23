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

install: libcredentials.a libcredentials.so credentials.o credentials.h
	#TODO

clean:
	$(RM) *.o *.so *.a
