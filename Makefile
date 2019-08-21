CSRC = $(wildcard *.c)
OBJ = $(CSRC:.c=.o)
DEP = $(OBJ:.o=.d)
SHELL := /bin/bash
BIN = "dnf-parser"
LEXFILE = host.l
BISONFILE = host.y
LEXOUT = lex.yy.c
BISONOUT = host.tab.c

#LDFLAGS = -lGL -lglut -lpng -lz -lm -lfl
LDFLAGS = -lfl
CC = gcc

.PHONY: cleandep cleanO clean check all parser

all: $(OBJ)
	@test -s ./$(LEXOUT)  && test -s ./$(BISONOUT) \
	|| { echo "Error: parser files are not found, run 'make parser' first.." ; exit 1; }
	clear
	$(CC) -o $(BIN) $^ $(LDFLAGS)

parser:
	flex -dl $(LEXFILE); \
	bison -tdv $(BISONFILE)

-include $(DEP)

%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

cleandep:
	rm -f $(DEP)

cleanO:
	rm -f $(OBJ)
	rm -f *.yy.c *.output
	rm -f *.tab.c *.tab.h
	rm -f ./$(BIN)

clean: cleandep cleanO
