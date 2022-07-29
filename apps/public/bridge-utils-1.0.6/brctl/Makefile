
KERNEL_HEADERS=-I../../../linux/include

#CC=mips-linux-gcc
CFLAGS= -Wall -s -Os
LDFLAGS=-s -Os
INCLUDE=-I../libbridge $(KERNEL_HEADERS) 
LIBS= -L ../libbridge -lbridge

ifeq ($(TCSUPPORT_MLD_SNOOPING),1)
     CFLAGS+= -DTCSUPPORT_MLD_SNOOPING
endif

ifeq ($(TCSUPPORT_IGMP_SNOOPING),1)
     CFLAGS+= -DTCSUPPORT_IGMP_SNOOPING
endif
ifeq ($(TCSUPPORT_IGMP_SNOOPING_V3),1)
     CFLAGS+= -DTCSUPPORT_IGMP_SNOOPING_V3
endif

prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man

INSTALL=/usr/bin/install -c


common_SOURCES= brctl_cmd.c brctl_disp.c 
brctl_SOURCES=  brctl.c $(common_SOURCES)

common_OBJECTS= $(common_SOURCES:.c=.o)
brctl_OBJECTS= $(brctl_SOURCES:.c=.o)

OBJECTS= $(common_OBJECTS) $(brctl_OBJECTS)

PROGRAMS= brctl 


all:	$(PROGRAMS)

install: $(PROGRAMS)
	mkdir -p $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(sbindir)

brctl:	$(brctl_OBJECTS) ../libbridge/libbridge.a
	$(CC) $(LDFLAGS) $(brctl_OBJECTS) $(LIBS) -o brctl
	$(STRIP) --strip-unneeded $(PROGRAMS)
%.o: %.c brctl.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< 

clean:
	rm -f *.o brctl core

