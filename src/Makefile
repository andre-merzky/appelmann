
CC      = gcc
CCFLAGS = -g -c -m64
LD      = gcc
LDFLAGS = -g -m64 -lpvm3 -lX11 -ldl

all: mandelmaster mandelslave

mandelmaster.o: mandelmaster.c
	$(CC) -o $@ $< $(CCFLAGS)

mandelslave.o: mandelslave.c
	$(CC) -o $@ $< $(CCFLAGS)

mandelmaster: mandelmaster.o
	$(LD) -o $@ $< $(LDFLAGS)

mandelslave:  mandelslave.o
	$(LD) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o mandelmaster mandelslave

