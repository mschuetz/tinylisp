#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "base.h"
#include "symbol_table.h"

struct object * car(struct cons_cell * cc){
  return cc->car;
}

struct object * cdr(struct cons_cell * cc){
  return cc->cdr;
}

bool atom_p(struct object * o){
  return o!=nil && o->atom_p;
}

struct object * cons(struct object * o1, struct object * o2){
  struct object * o = (struct object *) malloc(sizeof(struct object));
  o->atom_p = false;
  struct cons_cell * cc = (struct cons_cell *) malloc(sizeof(struct cons_cell));
  o->data = cc;
  cc->car = o1;
  cc->cdr = o2;
  return o;
}

struct object * quote(char * s){
  return st_insert(s);
}

void print_cons(struct cons_cell * cc);
void print_atom(atom a);

void print(struct object * o){
  if (o==nil) {
    printf("nil");
    return;
  }

  if (o->atom_p)
    print_atom((atom) o->data);
  else
    print_cons((struct cons_cell *) o->data);
}

void print_cons(struct cons_cell * cc){
  printf("(");
  print(car(cc));

  if (atom_p(cdr(cc))) {
    printf(" . ");
    print(cdr(cc));
  } else {
    
  }

  printf(")");
}

void print_atom(atom a){
  printf("%s", st_id_to_name(a));
}

struct obj * read(){
  char *s;
  while(s = (char *)yylex())
}

