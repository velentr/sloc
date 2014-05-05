CC	?=	gcc
PREFIX	?=	/usr
MANDIR	=	$(PREFIX)/share/man
CFLAGS	=	-O2 -c -Wall -Wstrict-prototypes -pedantic
OBJECTS	=	sloc.o

CC_COLOR	=	\x1b[36m
LD_COLOR	=	\x1b[33m
NO_COLOR	=	\x1b[0m

all: sloc

sloc: $(OBJECTS)
	@echo -e "$(LD_COLOR)LD$(NO_COLOR)  sloc"
	@$(CC) $(OBJECTS) -o sloc

sloc.o: sloc.c sloc.h languages.h
	@echo -e "$(CC_COLOR)CC$(NO_COLOR)  sloc.o"
	@$(CC) $(CFLAGS) sloc.c

clean:
	@echo "cleaning..."
	@rm -rf *.o sloc

install: all
	@echo "Installing binary..."
	@install -Dm755 sloc $(DESTDIR)$(PREFIX)/bin/sloc
	@echo "Installing man page..."
	@install -Dm644 sloc.1 $(DESTDIR)$(MANDIR)/man1/sloc.1
