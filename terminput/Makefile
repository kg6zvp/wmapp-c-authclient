CC=gcc

# Default to dynamically linked library
default: dynamic

#make it clear that it's the static version
static: libterminput.a

#make it clear that this is the dynamically linked version
dynamic: libterminput.so

#############
# TermInput #
#############
terminput.o: terminput.h terminput.c
	$(CC) -I"." -c terminput.c -fpic

libterminput.a: terminput.o
	ar rcs libterminput.a terminput.o
	ranlib libterminput.a

libterminput.so: terminput.o
	$(CC) -I"." -shared terminput.o -o libterminput.so

clean:
	$(RM) *.o *.so

