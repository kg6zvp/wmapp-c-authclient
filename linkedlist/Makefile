CC=gcc

# Default to dynamically linked library
default: dynamic

#make it clear that it's the static version
static: liblinkedlist.a

#make it clear that this is the dynamically linked version
dynamic: liblinkedlist.so

##############
# LinkedList #
##############
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -I"." -c linkedlist.c -fpic

liblinkedlist.a: linkedlist.o
	ar rcs liblinkedlist.a linkedlist.o
	ranlib liblinkedlist.a

liblinkedlist.so: linkedlist.o
	$(CC) -I"." -shared linkedlist.o -o liblinkedlist.so

clean:
	$(RM) *.o *.so *.a
