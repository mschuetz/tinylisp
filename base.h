#ifndef __TINYLISP_H
#define __TINYLISP_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef int atom;

#define nil NULL

typedef struct _cons_cell cons_cell;
typedef struct _object object;

struct _object {
  bool atom_p;
  bool function_p;
  cons_cell * data;
  int symbol_index;
};

struct _cons_cell {
  object * car;
  object * cdr;
};

extern object * globals;
extern object * globals_end;

object * car(object * o);
object * cdr(object * o);
object * quote(object * o);
object * atom_p(object * o);
object * eq(object * o1, object * o2);
object * cons(object * o1, object * o2);
object * print(object * o);
object * reader();
object * sym(const char * name);
object * list(int len, ...);

object * null(object * o);
object * and(object * x, object * y);
object * not(object * o);
object * cadr(object * o);
object * caddr(object * o);
object * cadar(object * o);
object * caar(object * o);
object * caddar(object * o);
object * append(object * x, object * y);
object * pair(object * x, object * y);
object * assoc(object * x, object * y);
object * eval(object * e, object * a);

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
