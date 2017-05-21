CC=gcc

# Default to dynamically linked library
default: dynamic

#make it clear that it's the static version
static: libobjstring.a

#make it clear that this is the dynamically linked version
dynamic: libobjstring.so

#############
# ObjString #
#############
objstring.o: objstring.h objstring.c
	$(CC) -I"." -c objstring.c -fpic

libobjstring.a: objstring.o
	ar rcs libobjstring.a objstring.o
	ranlib libobjstring.a

libobjstring.so: objstring.o
	$(CC) -I"." -shared objstring.o -o libobjstring.so

clean:
	$(RM) *.o *.so *.a
