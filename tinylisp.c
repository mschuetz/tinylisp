#include <stdio.h>
#include <readline/readline.h>
#include "base.h"
#include "symbol_table.h"
#include "parser.h"

struct object * globals = nil;
struct object * globals_end = nil;

int main(int argc, char **argv){
  char * s;
  sym("nil"); sym("t");

  globals = cons(list(2, sym("t"), sym("t")), nil);
  globals_end = globals;
  while ((s = readline("tinylisp> "))) {
    print(eval(parse_string(s), globals));
    puts("");
    free(s);
  }
  return 0;
}


