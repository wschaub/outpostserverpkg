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

#install all dependency packages and set up lighttpd
#XXX! Possibly ubuntu specific, please test on at least regular debian
installdep:
	echo "installing dpendencies"
	aptitude install $(DEPENDS)
	lighty-enable-mod fastcgi fastcgi-php
	service lighttpd force-reload
	chown www-data:www-data /var/www

services:

install: services

