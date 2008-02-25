LIBFL = /opt/local/lib/libfl.a

flex:
	flex lisp_scanner.rl

.c.o:
	$(CC) $(INCFLAGS) $(CPPFLAGS) $(CFLAGS) -c $<

tinylisp: flex tinylisp.o symbol_table.o base.o lex.yy.o
	$(CC) -o tinylisp tinylisp.o symbol_table.o base.o lex.yy.o $(LIBFL)


run: tinylisp
	./tinylisp

clean: 
	rm tinylisp *.o

test: flex test_scanner.o lex.yy.o
	gcc -o test_scanner test_scanner.o lex.yy.o $(LIBFL)

all: tinylisp