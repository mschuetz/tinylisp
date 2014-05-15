#include <string.h>
#include <stdio.h>
#include "symbol_table.h"

static struct symbol_table st;

struct object * st_insert(char * name){
  struct object * o;
  if ((o = st_lookup(name)) != NULL)
    return o;

  struct symbol * sym = &st.symbols[st.size];
  o = &sym->object;
  o->atom_p = true;
  o->function_p = false;
  o->symbol_index = st.size;

  int length = strlen(name);
  strncpy(sym->name, name, length);

  st.size++;
  return o;
}

struct object * st_lookup(char * name){
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

