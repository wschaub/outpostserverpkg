.PHONY: common basestation remote
all: 
 
common:
	if [ ! -d tmp ]; then mkdir tmp; fi
	cp -rp common/* tmp

relocate: 
ifneq ($(WWWROOT),/var/www)
	if [ -d tmp/var/www ]; then mkdir -p tmp/$(WWWROOT); cp -rp tmp/var/www/* tmp/$(WWWROOT); rm -rf tmp/var/www; fi
endif

basestation: common
	cp -rp basestation/* tmp

remote: common
	cp -rp remote/* tmp 

clean:
	rm -rf tmp

install: $(TYPE) relocate tmp
	  chown -R root:root tmp
	  chown -R $(WWW_USER):$(WWW_GROUP) tmp/$(WWWROOT)
	  cp -rp tmp/* $(DESTDIR)/
