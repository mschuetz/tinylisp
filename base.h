#ifndef __TINYLISP_H
#define __TINYLISP_H

#include <stdbool.h>
#include <stdlib.h>

typedef int atom;

#define nil NULL

struct object {
  bool atom_p;
  void * data;
};

struct cons_cell {
  struct object * car;
  struct object * cdr;
};

struct object * car(struct cons_cell * cc);
struct object * cdr(struct cons_cell * cc);
struct object * quote(char * s);
struct object * cons(struct object * o1, struct object * o2);
void print(struct object * o);
struct object * reader();

#endif
