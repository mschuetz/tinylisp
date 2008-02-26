#include <stdio.h>
#include <readline/readline.h>
#include "base.h"
#include "symbol_table.h"
#include "parser.h"

int main(int argc, char **argv){
  char * s;
  while (s = readline("tinylisp> ")) {
    print(eval(parse_string(s), nil));
    puts("");
    free(s);
  }
  return 0;
}

