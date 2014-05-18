#include <stdio.h>
#include <readline/readline.h>
#include "base.h"
#include "symbol_table.h"
#include "parser.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

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

#pragma clang diagnostic pop
