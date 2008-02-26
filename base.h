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
struct object * atom_p(struct object * o);
struct object * eq(struct object * o1, struct object * o2);
struct object * cons(struct object * o1, struct object * o2);
void print(struct object * o);
struct object * reader();
struct object * sym(char * name);
struct object * list(int len, ...);

struct object * null(struct object * o);
struct object * and(struct object * x, struct object * y);
struct object * not(struct object * o);

struct object * cadr(struct object * o);
struct object * caddr(struct object * o);
struct object * caar(struct object * o);
struct object * caddar(struct object * o);
struct object * append(struct object * x, struct object * y);
struct object * pair(struct object * x, struct object * y);
#endif
