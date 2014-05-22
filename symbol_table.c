#include <string.h>
#include <stdio.h>
#include "symbol_table.h"
#include "base.h"

static symbol_table st;

object * st_insert(const char * name){
  object * o;
  if ((o = st_lookup(name)) != NULL)
    return o;

  check(st.size < SYMBOL_TABLE_SIZE, "symbol table size exceeded");
  symbol * sym = &st.symbols[st.size];
  o = &sym->object;
  o->atom_p = true;
  o->function_p = false;
  o->symbol_index = st.size;

  size_t length = strlen(name);
  strncpy(sym->name, name, length);

  st.size++;
  return o;
}

object * st_lookup(const char * name){
  int i;
  for (i=0; i<st.size; i++){
    if (strcmp(name, st.symbols[i].name) == 0)
      return &st.symbols[i].object;
  }
  return NULL;
}

char * st_id_to_name(atom id){
  return st.symbols[id].name;
}

