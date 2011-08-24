SERVICEPREFIX=/opt/outpostserver
_CFLAGS=-g -Ilibmodbus-2.0.4
_LDFLAGS=-Llibmodbus-2.0.4/modbus/.libs -Wl,-rpath=$(SERVICEPREFIX)/lib -lmodbus
#Default make rule to build all commands
all: sunsaver sunsaverEEPROM sunsaverlog

clean:
	rm -rf libmodbus-2.0.4
	rm -f *.o src/*.o sunsaver sunsaverEEPROM sunsaverlog modbus

libmodbus-2.0.4:
	tar xzvf libmodbus-2.0.4.tar.gz

modbus: libmodbus-2.0.4
	cd libmodbus-2.0.4 && ./configure --prefix $(DESTDIR)$(SERVICEPREFIX) && $(MAKE) 
	touch modbus

sunsaver: src/sunsaver.c modbus
	$(CC) $(_CFLAGS) -o sunsaver src/sunsaver.c $(_LDFLAGS)

sunsaverEEPROM: src/sunsaverEEPROM.c modbus
	$(CC) $(_CFLAGS) -o sunsaverEEPROM src/sunsaverEEPROM.c $(_LDFLAGS)

sunsaverlog: src/sunsaverlog.c modbus
	$(CC) $(_CFLAGS) -o sunsaverlog src/sunsaverlog.c $(_LDFLAGS)
	
install: sunsaver sunsaverEEPROM sunsaverlog
	cd libmodbus-2.0.4 && $(MAKE) install
	install -D sunsaver $(DESTDIR)$(SERVICEPREFIX)/bin/sunsaver
	install -D sunsaverEEPROM $(DESTDIR)$(SERVICEPREFIX)/bin/sunsaverEEPROM
	install -D sunsaverlog $(DESTDIR)$(SERVICEPREFIX)/bin/sunsaverlog
