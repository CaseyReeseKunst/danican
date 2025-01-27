# Danican makefile
# Copyright (c) Christian Obrecht 2002

PREFIX = /usr/local/
BINDIR = $(PREFIX)bin/
LIBDIR = $(PREFIX)lib/danican/
MANDIR = $(PREFIX)man/man6/

CC = gcc
CFLAGS = -O2 `gtk-config --cflags` -DLIBDIR='"$(LIBDIR)"'
OBJECTS = base.o board.o callback.o eval.o game.o init.o main.o play.o
TEXTS = base.pdn rules.txt manual.txt COPYING

all: danican

danican: $(OBJECTS)
	$(CC) -o $@ `gtk-config --libs` $(OBJECTS)

base.o: base.c board.h base.h

board.o: board.c board.h

callback.o: callback.c board.h game.h main.h init.h play.h callback.h

eval.o: eval.c board.h eval.h

game.o: game.c board.h eval.h base.h game.h

init.o: init.c board.h game.h main.h play.h callback.h

main.o: main.c board.h base.h game.h main.h init.h callback.h

play.o: play.c board.h game.h main.h

.c.o:
	$(COMPILE.c) $< $(OUTPUT_OPTION)

install: danican danican.6 $(TEXTS)
	install -s danican $(BINDIR) ;\
	install -d $(LIBDIR) ;\
	install -m 0644 $(TEXTS) $(LIBDIR) ;\
	install -m 0644 danican.6 $(MANDIR)

uninstall:
	rm -f $(BINDIR)danican ;\
	rm -rf $(LIBDIR)

clean:
	rm -f *.o danican
