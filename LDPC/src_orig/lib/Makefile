
RM= rm -f

CFLAGS = -DRAND_FILE=\"`pwd`/randfile\" -fPIC -Wall -Wextra -O2 -g # C flags
LDFLAGS = -shared  # linking flags

SRCS = alloc.c blockio.c channel.c\
       check.c dec.c distrib.c\
       enc.c intio.c mod2convert.c\
       mod2dense.c mod2sparse.c\
       open.c rand.c rcode.c


OBJS = $(SRCS:.c=.o)

TARGET_LIB = libLdpc1.so


.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)

 
