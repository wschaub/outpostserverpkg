#Default make rule to build all commands
all: powerbutton

clean:
	rm -f *.o powerbutton


powerbutton: powerbutton.c 
	$(CC) $(CFLAGS) -o powerbutton powerbutton.c $(LDFLAGS)

	
install: powerbutton
	install -D powerbutton $(DESTDIR)$(SERVICEPREFIX)/bin/powerbutton
