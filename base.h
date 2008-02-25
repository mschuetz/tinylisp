#ifndef __TINYLISP_H
#define __TINYLISP_H

#include <stdbool.h>
#include <stdlib.h>

typedef int atom;

#define nil NULL

struct object {
  bool atom_p;
  bool function_p;
  void * data; // either (atom *) or (struct cons_cell *)
};

struct cons_cell {
  struct object * car;
  struct object * cdr;
};

struct object * car(struct object * o);
struct object * cdr(struct object * o);
struct object * quote(struct object * o);
bool atom_p(struct object * o);
bool eq(struct object * o1, struct object * o2);
struct object * cons(struct object * o1, struct object * o2);
void print(struct object * o);
struct object * reader();
struct object * sym(char * name);
struct object * list(int len, ...);
#endif
