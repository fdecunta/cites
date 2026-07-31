CC = cc
FLAGS = -pedantic -Wall -Wextra -Werror -Wconversion

BINDIR = /usr/local/bin

SRC = cites.c
PROG = cites

$(PROG): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(PROG)

install: $(PROG)
	cp $(PROG) $(BINDIR)

remove: 
	rm -f $(BINDIR)/$(PROG)

.PHONY: install remove 
