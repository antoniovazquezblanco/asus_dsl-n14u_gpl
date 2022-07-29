CC=gcc
CFLAGS += -DTRENDCHIP
ifneq ($(US_PROFILE),)
CFLAGS += -D$(US_PROFILE)
endif

ifeq ($(NAND),1)
CFLAGS += -DTCSUPPORT_BB_NAND
endif

OBJS=trx.o
EXECS=trx
all: trx

.c.o:
	$(CC) -c $(CFLAGS) $<

trx: $(OBJS) $(MIPSLIBS)
	$(CC) $(OBJS) -o $@

clean:
	rm -f *.o *.a $(EXECS)

.PHONY : release
release:
	rm -rf *.c
	mv Makefile.release Makefile
