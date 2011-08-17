all:
	if [ ! -d tmp ]; then mkdir tmp; fi
	cp -rp shared wiki tmp/

install:
	if [ ! -d $(DESTDIR)$(WWWROOT) ]; then mkdir -p $(DESTDIR)$(WWWROOT); \
chown $(WWW_USER):$(WWW_GROUP) $(DESTDIR)$(WWWROOT); fi  
	chown -R $(WWW_USER):$(WWW_GROUP) tmp
	cp -rp tmp/* $(DESTDIR)$(WWWROOT)
	if [ -f $(DESTDIR)$(WWWROOT)/wiki/conf/local.php ]; then touch $(DESTDIR)\
$(WWWROOT)/wiki/conf/local.php; fi
	
clean:
	rm -rf tmp
