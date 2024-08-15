CC = gcc
CFLAGS=-g -Wall -ggdb3 -c
CFILES=lex.yy.c main.c rustgram.tab.c tree.c ytab.c utils.c symtab.c type.c tac.c finalc.c

all: bison flex link clean

bison: rustgram.y
	bison rustgram.y
	bison -d rustgram.y

flex: rustlex.l
	flex rustlex.l

lex.yy.o: lex.yy.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) lex.yy.c

main.o: main.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h finalc.h
	$(CC) $(CFLAGS) main.c

rustgram.tab.o: rustgram.tab.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) rustgram.tab.c

tree.o: tree.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) tree.c

ytab.o: ytab.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) ytab.c

utils.o: utils.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) utils.c

symtab.o: symtab.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) symtab.c

type.o: type.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) type.c

tac.o: tac.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) tac.c

finalc.o: finalc.c ytab.h tree.h rustgram.tab.h utils.h symtab.h type.h tac.h
	$(CC) $(CFLAGS) finalc.c

link: lex.yy.o main.o rustgram.tab.o tree.o ytab.o utils.o symtab.o type.o tac.o finalc.o
	$(CC) lex.yy.o main.o rustgram.tab.o tree.o ytab.o utils.o symtab.o type.o tac.o finalc.o -o fec

clean: lex.yy.c lex.yy.o main.o tree.o ytab.o utils.o rustgram.tab.c rustgram.tab.o rustgram.tab.h symtab.o type.o tac.o finalc.o
	rm -f lex.yy.c lex.yy.o main.o tree.o ytab.o utils.o rustgram.tab.c rustgram.tab.o rustgram.tab.h symtab.o type.o tac.o finalc.o