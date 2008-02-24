#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include "base.h"
struct symbol {
  struct object object;
  char name[128];
};

struct symbol_table {
  struct symbol symbols[2048];
  int size;
};

struct object * st_insert(char * name);
struct object * st_lookup(char * name);
char * st_id_to_name(int id);

#endif
