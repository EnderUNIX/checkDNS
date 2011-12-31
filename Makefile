#CC = gcc -ansi -pedantic -DDEBUG -g
CC = gcc -ansi -pedantic

INSTALL = install

OBJS = loadconfig.o langcfg.o dns.o html.o tools.o main.o

compile: $(OBJS)
	$(CC) -o checkdns $(OBJS)

install:
	cp checkdns /usr/local/bin/checkdns

clean:
	rm -f $(OBJS) checkdns

uninstall: clean
	rm /usr/local/bin/checkdns

all: compile install clean
