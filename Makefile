LIBFL = /opt/local/lib/libfl.a

flex:
	flex lisp_scanner.rl

.c.o:
	$(CC) $(INCFLAGS) $(CPPFLAGS) $(CFLAGS) -c $<

tinylisp: flex tinylisp.o symbol_table.o base.o lex.yy.o parser.o
	$(CC) -g -lreadline -o tinylisp tinylisp.o symbol_table.o base.o lex.yy.o parser.o $(LIBFL)

run: tinylisp
	./tinylisp

clean: 
	rm tinylisp *.o || true

test: flex test_scanner.o lex.yy.o
	gcc -o test_scanner test_scanner.o lex.yy.o $(LIBFL)

all: tinylisp
