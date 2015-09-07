CC	?=	gcc
YACC	?=	bison
LEX	?=	flex
PREFIX	?=	/usr
MANDIR	=	$(PREFIX)/share/man
# CFLAGS	=	-O2 -DNDEBUG -c
CFLAGS	=	-g -O0 -Wall -Wextra -c
LDFLAGS	=	-O2
OBJECTS	=	config.o count.o langspec.o sloc.o parser.o scanner.o
BIN	=	sloc

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

config.o: config.c config.h langspec.h
	$(CC) $(CFLAGS) $<

count.o: count.c count.h langspec.h
	$(CC) $(CFLAGS) $<

langspec.o: langspec.c langspec.h
	$(CC) $(CFLAGS) $<

sloc.o: sloc.c
	$(CC) $(CFLAGS) $<

parser.c: parser.yy
	$(YACC) -d -o $@ $<

parser.h: parser.yy
	$(YACC) -d -o $@ $<

parser.o: parser.c parser.h config.h
	$(CC) -c $<

scanner.c: scanner.l
	$(LEX) -o $@ $<

scanner.o: scanner.c parser.h
	$(CC) -c $<

clean:
	rm -rf *.o $(BIN) scanner.c parser.{c,h}

install: all
	install -Dm755 $(BIN) $(DESTDIR)$(PREFIX)/bin/$(BIN)
	install -Dm644 $(BIN).1 $(DESTDIR)$(MANDIR)/man1/$(BIN).1
