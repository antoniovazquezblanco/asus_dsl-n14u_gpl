# PPP top-level Makefile for Linux.


#CC=mips-linux-gcc
LD=mips-linux-ld
STRIP=mips-linux-strip

all:
	cd pppd; make -C plugins
	make -C pppd
	
install:# $(BINDIR) $(MANDIR)/man8 install-progs install-etcppp
	$(STRIP) pppd/pppd		
	$(STRIP) pppd/plugins/libpppoatm.so
	$(STRIP) pppd/plugins/pppoe/libpppoe.so
	cp -a pppd/pppd $(FILESYSTEM_DIR)/usr/bin/
	cp -a pppd/plugins/libpppoatm.so $(FILESYSTEM_DIR)/lib/
	cp -a pppd/plugins/pppoe/libpppoe.so $(FILESYSTEM_DIR)/lib/

install-progs:
	cd chat; $(MAKE) BINDIR=$(BINDIR) MANDIR=$(MANDIR) $(MFLAGS) install
	cd pppd/plugins; $(MAKE) BINDIR=$(BINDIR) MANDIR=$(MANDIR) $(MFLAGS) install
	cd pppd; $(MAKE) BINDIR=$(BINDIR) MANDIR=$(MANDIR) $(MFLAGS) install
	cd pppstats; $(MAKE) BINDIR=$(BINDIR) MANDIR=$(MANDIR) $(MFLAGS) install
	cd pppdump; $(MAKE) BINDIR=$(BINDIR) MANDIR=$(MANDIR) $(MFLAGS) install

install-etcppp: $(ETCDIR) $(ETCDIR)/options $(ETCDIR)/pap-secrets \
	$(ETCDIR)/chap-secrets

$(ETCDIR)/options:
	$(INSTALL) -c -m 644 etc.ppp/options $@
$(ETCDIR)/pap-secrets:
	$(INSTALL) -c -m 600 etc.ppp/pap-secrets $@
$(ETCDIR)/chap-secrets:
	$(INSTALL) -c -m 600 etc.ppp/chap-secrets $@

$(BINDIR):
	$(INSTALL) -d -m 755 $@
$(MANDIR)/man8:
	$(INSTALL) -d -m 755 $@
$(ETCDIR):
	$(INSTALL) -d -m 755 $@

clean:
	rm -f `find . -name '*.[oas]' -print`
	rm -f `find . -name 'core' -print`
	rm -f `find . -name '*~' -print`
	cd chat; $(MAKE) clean
	cd pppd/plugins; $(MAKE) clean
	cd pppd; $(MAKE) clean
	cd pppstats; $(MAKE) clean
	cd pppdump; $(MAKE) clean

dist-clean:	clean
	rm -f Makefile `find . -name Makefile -print`

#kernel:
#	cd linux; ./kinstall.sh
