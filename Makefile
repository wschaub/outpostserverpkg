#Current version of the outpost server package.
export VERSION=0.5

#Current working directory, used by the tarpkg target
export CWD=`pwd`

#LSB information for generating tar packages
DISTRIB_ID=`lsb_release -s -i`
DISTRIB_CODENAME=`lsb_release -s -c`
#Directory prefix to put bundeled service programs
export SERVICEPREFIX=/opt/outpostserver

#Location of the web root to install web apps/content into
export WWWROOT=/var/www

export WWW_USER=www-data

export WWW_GROUP=www-data

#the type of system we will be installing (must be a name of
#a directory under server/ but must NOT be "common"
#this should be basestation or remote currently
export TYPE=basestation

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
	cd server && $(MAKE) install

#Generate a set of generic tar based packages
#that should work on any distro.
tarpkg: clean
	#prepare package dir
	mkdir -p tmp/fakeroot
	mkdir -p tmp/pkg
	#copy over install script and support files
	cp misc/installpkg.sh tmp/pkg
	cp misc/TARPKGREADME tmp/pkg
	cp misc/debpostinst tmp/pkg
	cp misc/postinst tmp/pkg
	echo $(DEPENDS) >tmp/DEPS
	echo "SERVICEPREFIX=$(SERVICEPREFIX)" >tmp/vars
	echo "WWWROOT=$(WWWROOT)" >>tmp/vars
	echo TARGET_DISTRIB_ID=$(DISTRIB_ID) >>tmp/vars
	echo TARGET_DISTRIB_CODENAME=$(DISTRIB_CODENAME) >>tmp/vars
	#Builld the -bin package for our current type
	cp tmp/DEPS tmp/vars tmp/pkg
	$(MAKE) DESTDIR=$(CWD)/tmp/fakeroot/
	fakeroot $(MAKE) DESTDIR=$(CWD)/tmp/fakeroot/ _tarpkgbin
	cd tmp && mv pkg outpostserverpkg-$(VERSION)-$(TYPE)-bin 
	cd tmp && tar czvf ../../outpostserverpkg-$(VERSION)-$(TYPE)-bin.tgz \
		outpostserverpkg-$(VERSION)-$(TYPE)-bin

contenttarpkg: clean
	#Build the content package
	#prepare package dir
	mkdir -p tmp/fakeroot
	mkdir -p tmp/pkg
	#copy over install script and support files
	cp misc/installpkg.sh tmp/pkg
	cp misc/postinst tmp/pkg
	cp misc/TARPKGREADME tmp/pkg/README
	echo "SERVICEPREFIX=$(SERVICEPREFIX)" >tmp/vars
	echo "WWWROOT=$(WWWROOT)" >>tmp/vars
	echo TARGET_DISTRIB_ID=$(DISTRIB_ID) >>tmp/vars
	echo TARGET_DISTRIB_CODENAME=$(DISTRIB_CODENAME) >>tmp/vars
	cp tmp/vars tmp/pkg
	touch tmp/pkg/CONTENT
	fakeroot $(MAKE) DESTDIR=$(CWD)/tmp/fakeroot _tarpkgcontent
	cd tmp && mv pkg outpostserverpkg-$(VERSION)-content 
	cd tmp && tar czvf ../../outpostserverpkg-$(VERSION)-content.tgz \
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
	cd server && $(MAKE) clean
	cd content && $(MAKE) clean
#	cd client && $(MAKE) clean
