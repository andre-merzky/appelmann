
CC      = gcc
CCFLAGS = -g -c
LD      = gcc
LDFLAGS = -g -lpvm -lX11

all: mandelmaster mandelslave

mandelmaster.o: mandelmaster.c
	$(CC) $(CCFLAGS) -o $@ $<

mandelslave.o: mandelslave.c
	$(CC) $(CCFLAGS) -o $@ $<

mandelmaster: mandelmaster.o
	$(LD) $(LDFLAGS) -o $@ $<

mandelslave:  mandelslave.o
	$(LD) $(LDFLAGS) -o $@ $<


