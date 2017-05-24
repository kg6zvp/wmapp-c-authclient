CC = gcc

CFLAGS += $(shell pkg-config --cflags mc_wmappauth)
LDFLAGS += $(shell pkg-config --libs mc_wmappauth)

STATIC_CFLAGS += $(shell pkg-config --cflags mc_wmappauth_static)
STATIC_LDFLAGS += $(shell pkg-config --libs mc_wmappauth_static)

default: static

static: sample.c
	$(CC) $(STATIC_CFLAGS) $(STATIC_LDFLAGS) sample.c -o static

#$(CC) $(CFLAGS) $(LDFLAGS) sample.c libwmappauth.a -o sample_static

static_debug: sample.c
	$(CC) $(CFLAGS) $(LDFLAGS) -g sample.c -o sample_static_debug -lm

dynamic: sample.c
	$(CC) sample.c $(CFLAGS) $(LDFLAGS) -o dynamic -lm

prepare:
	sudo dnf install libcurl-devel libnghttp2-devel glibc-static

clean:
	$(RM) *.o *.so *.a dynamic static static_debug
