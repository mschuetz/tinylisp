#ifndef __TINYLISP_H
#define __TINYLISP_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef int atom;

#define nil NULL

typedef struct {
  bool atom_p;
  bool function_p;
  cons_cell * data;
  int symbol_index;
} object;

typedef struct {
  object * car;
  object * cdr;
} cons_cell;

extern object * globals;
extern object * globals_end;

object * car(struct object * o);
object * cdr(struct object * o);
object * quote(struct object * o);
object * atom_p(struct object * o);
object * eq(struct object * o1, struct object * o2);
object * cons(struct object * o1, struct object * o2);
object * print(struct object * o);
object * reader();
object * sym(const char * name);
object * list(int len, ...);

object * null(struct object * o);
object * and(struct object * x, struct object * y);
object * not(struct object 
object * cadr(struct object * o);
object * caddr(struct object * o);
object * cadar(struct object * o);
object * caar(struct object * o);
object * caddar(struct object * o);
object * append(struct object * x, struct object * y);
object * pair(struct object * x, struct object * y);
object * assoc(struct object * x, struct object * y);
object * eval(struct object * e, struct object * a);

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, "%s: " fmt "\n", __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
#define LOG(fmt, ...)
#endif

#define check(cond, error_fmt, ...) {\
  if (!(cond)) {\
    fprintf(stderr, "%s: (%s) == false. " error_fmt "\n", __PRETTY_FUNCTION__, #cond, ##__VA_ARGS__);\
    exit(EXIT_FAILURE);\
  }\
}

#define check_not_nil(p, error_fmt, ...) (check(p!=nil, error_fmt, ##__VA_ARGS__))

#endif
