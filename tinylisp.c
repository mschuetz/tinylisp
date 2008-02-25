#include <stdio.h>
#include "base.h"
#include "symbol_table.h"
#include "parser.h"

int main(int argc, char **argv){
  print(parse()); puts("");
  return 0;
}

