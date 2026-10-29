CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fvisibility=hidden -fPIC
LDFLAGS=-shared -Wl,--no-undefined
CPPFLAGS=-D_DEFAULT_SOURCE
SRCS=src/malloc.c src/allocation.c src/page_manage.c
OBJS=${SRCS:.c=.o}
TARGET=libmalloc.so
	
library: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

check:
	$(CC) -lcriterion tests/tests.c -o test_suite
	./tests/tests.sh

clean:
	$(RM) $(TARGET)
	$(RM) $(OBJS)
	$(RM) test_suite
	$(RM) tests/tests.o

