#Current version of the outpost server package.
VERSION=0.5
#Current working directory, used by the tarpkg target
CWD=`pwd`

#Directory prefix to put bundeled service programs
SERVICEPREFIX=/opt/outpostserver

#Location of the web root to install web apps/content into
WWWROOT=/var/www

#the type of system we will be installing (must be a name of
#a directory under server/ but must NOT be "common"
#this should be basestation or remote currently
TYPE=basestation

#packages that need to be installed before we run the final install steps
DEPENDS="dnsmasq lighttpd php5-cgi php5-sqlite samba vim"

.PHONY: services server content

all: services server
	@echo "don't forget to run make installdep if you aren't building a package"
#install all dependency packages and set up lighttpd
#XXX! Possibly ubuntu specific, please test on at least regular debian
installdep:
	echo "installing dpendencies"
	aptitude install $(DEPENDS)
	lighty-enable-mod fastcgi fastcgi-php
	service lighttpd force-reload
	chown www-data:www-data /var/www

services:
	cd services && $(MAKE) 

server:
	cd server && $(MAKE)

content:
	cd content && $(MAKE)

contentinstall: content
	cd content && $(MAKE) install

install: services server
	cd services && $(MAKE) install
#	cd server && $(MAKE) install

#Generate a set of generic tar based packages
#that should work on any distro.
tarpkg: clean
	mkdir -p tmp/fakeroot
	mkdir -p tmp/pkg
	cp misc/installpkg.sh tmp/pkg
	echo $(DEPENDS) >tmp/DEPS
	echo "SERVICEPREFIX=$(SERVICEPREFIX)" >tmp/vars
	echo "WWWROOT=$(WWWROOT)" >>tmp/vars
	cat /etc/lsb-release | awk '{print "TARGET_"$$0}' >>tmp/vars
	cp tmp/DEPS tmp/vars tmp/pkg
	$(MAKE) DESTDIR=$(CWD)/tmp/fakeroot/
	fakeroot $(MAKE) DESTDIR=$(CWD)/tmp/fakeroot/ _tarpkgbin
	cd tmp && mv pkg outpostserverpkg-$(VERSION)-bin 
	cd tmp && tar czvf ../outpostserverpkg-$(VERSION)-bin.tgz \
		outpostserverpkg-$(VERSION)-bin
	rm -rf tmp/fakeroot
	mkdir tmp/fakeroot
	mkdir tmp/pkg
	cp misc/installpkg.sh tmp/pkg
	cp tmp/vars tmp/pkg
	touch tmp/pkg/CONTENT
	fakeroot $(MAKE) DESTDIR=$(CWD)/tmp/fakeroot _tarpkgcontent
	cd tmp && mv pkg outpostserverpkg-$(VERSION)-content 
	cd tmp && tar czvf ../outpostserverpkg-$(VERSION)-content.tgz \
		outpostserverpkg-$(VERSION)-content


#we need a seperate target so these commands can run under the context of
#fakeroot
_tarpkgbin:
	$(MAKE) install
	cd tmp/fakeroot && tar cvf ../pkg/image.tar .

#same as above only this makes the content image instead.
_tarpkgcontent:
	$(MAKE) contentinstall
	cd tmp/fakeroot && tar cvf ../pkg/image.tar .

debpkg:
clean:
	rm -rf tmp *.tgz
	cd services && $(MAKE) clean
#	cd server && $(MAKE) clean
#	cd content && $(MAKE) clean
#	cd client && $(MAKE) clean
