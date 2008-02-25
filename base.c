#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "base.h"
#include "symbol_table.h"

struct object * car(struct object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return ((struct cons_cell *)o->data)->car;
    else
      return nil;
  }
  fprintf(stderr, "not a list\n");
  exit(1);
}

struct object * cdr(struct object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return ((struct cons_cell *)o->data)->cdr;
    else
      return nil;
  }
  fprintf(stderr, "not a list\n");
  exit(1);
}

bool atom_p(struct object * o){
  return o!=nil && o->atom_p;
}

bool eq(struct object * o1, struct object * o2){
  return (atom_p(o1) && atom_p(o2) && o1==o2) || (o1==o2 && o2==nil);
}

struct object * cond(int len, ...) {
  va_list ap;
  struct object * o;
  struct object * val = nil;
  int i;
  
  va_start(ap, len);
  for (i=0;i<len;i++){
    o = va_arg(ap, struct object *);
    if (car(o)!=nil) {
      val = cdr(o);
      break;
    }
  }
  va_end(ap);
  return val;
}

struct object * list(int len, ...) {
  va_list ap;
  struct object * start = nil;
  struct object * cur = nil;
  struct object * o = nil;
  int i;
  start = cons(nil, nil);
  cur = start;
  va_start(ap, len);
  for (i=0;i<len;i++){
    o = va_arg(ap, struct object *);
    ((struct cons_cell *)cur->data)->car = o;
    if (i==len-1)
      ((struct cons_cell *)cur->data)->cdr = nil;
    else
      ((struct cons_cell *)cur->data)->cdr = cons(nil, nil);
    cur = cdr(cur);
  }
  va_end(ap);
  return start;
}

struct object * cons(struct object * o1, struct object * o2){
  struct object * o = (struct object *) malloc(sizeof(struct object));
  o->atom_p = false;
  o->function_p = false;
  struct cons_cell * cc = (struct cons_cell *) malloc(sizeof(struct cons_cell));
  o->data = cc;
  cc->car = o1;
  cc->cdr = o2;
  return o;
}

struct object * quote(struct object * o){
  return o;
}

struct object * lambda(struct object * o){
  
}

void print_cons(struct object * o);
void print_atom(atom a);

void print(struct object * o){
  if (o==nil) {
    printf("nil");
    return;
  }

  if (o->atom_p)
    print_atom((atom) o->data);
  else
    print_cons(o);
}

void print_cons(struct object * o){
  /*  printf("(");
  print(car(cc));
  printf(" . ");
  print(cdr(cc));
  printf(")");*/

  printf("(");
  do {
    print(car(o));
    if (cdr(o)!=nil)
      printf(" ");
  } while (o = cdr(o));
  printf(")");
}

void print_atom(atom a){
  printf("%s", st_id_to_name(a));
}

struct object * sym(char * name){
  return st_insert(name);
}

