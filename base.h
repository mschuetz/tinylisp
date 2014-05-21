#ifndef __TINYLISP_H
#define __TINYLISP_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef int atom;

#define nil NULL

struct object {
  bool atom_p;
  bool function_p;
  struct cons_cell * data;
  int symbol_index;
};

struct cons_cell {
  struct object * car;
  struct object * cdr;
};

extern struct object * globals;
extern struct object * globals_end;

struct object * car(struct object * o);
struct object * cdr(struct object * o);
struct object * quote(struct object * o);
struct object * atom_p(struct object * o);
struct object * eq(struct object * o1, struct object * o2);
struct object * cons(struct object * o1, struct object * o2);
struct object * print(struct object * o);
struct object * reader();
struct object * sym(const char * name);
struct object * list(int len, ...);

struct object * null(struct object * o);
struct object * and(struct object * x, struct object * y);
struct object * not(struct object * o);

struct object * cadr(struct object * o);
struct object * caddr(struct object * o);
struct object * cadar(struct object * o);
struct object * caar(struct object * o);
struct object * caddar(struct object * o);
struct object * append(struct object * x, struct object * y);
struct object * pair(struct object * x, struct object * y);
struct object * assoc(struct object * x, struct object * y);
struct object * eval(struct object * e, struct object * a);

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
