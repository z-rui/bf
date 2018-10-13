CC=gcc
CFLAGS=-Wall -Wextra -ggdb3
CFLAGS+=-DNDEBUG -O2

bf: bf.o exec.o load.o dump.o to_c.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o

.PHONY: all clean

bf.o: bf.c bf.h
dump.o: dump.c bf.h bf_inst.h
exec.o: exec.c bf.h bf_inst.h
load.o: load.c bf.h bf_inst.h
to_c.o: to_c.c bf.h bf_inst.h
