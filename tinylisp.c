#include <stdio.h>
#include "base.h"
#include "symbol_table.h"

int main(int argc, char **argv){
  st_insert("nil");
  st_insert("t");
  struct object * list;
  list = cons(quote("a"), cons(quote("b"), cons(quote("c"), nil)));
  print(cons(quote("a"), quote("b"))); puts("");
  print(list);
  return 0;
}

