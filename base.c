#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <readline/readline.h>
#include "base.h"
#include "symbol_table.h"

struct object * car(struct object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return o->data->car;
    else
      return nil;
  }
  check(false, "argument not a list");
}

struct object * cdr(struct object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return o->data->cdr;
    else
      return nil;
  }
  check(false, "argument not a list");
}

struct object * atom_p(struct object * o){
  if (o!=nil && o->atom_p)
    return sym("t");
  else
    return nil;
}

struct object * eq(struct object * o1, struct object * o2){
  if ((atom_p(o1) && atom_p(o2) && o1==o2) || (o1==o2 && o2==nil))
    return sym("t");
  else
    return nil;
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
    cur->data->car = o;
    if (i==len-1)
      cur->data->cdr = nil;
    else
      cur->data->cdr = cons(nil, nil);
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

static void print_cons(struct object * o);
static void print_atom(atom a);

struct object * print(struct object * o){
  if (o==nil) {
    printf("nil");
    return nil;
  }

  if (o->atom_p)
    print_atom(o->symbol_index);
  else
    print_cons(o);

  return nil;
}

static void print_cons(struct object * o){
  /*  printf("(");
  print(car(o));
  printf(" . ");
  print(cdr(o));
  printf(")");*/

  printf("(");
  do {
    print(car(o));
    if (cdr(o)!=nil)
      printf(" ");
  } while ((o = cdr(o)));
  printf(")");
}

static void print_atom(atom a){
  printf("%s", st_id_to_name(a));
}

struct object * null(struct object * o){
  return eq(o, nil);
}

struct object * and(struct object * x, struct object * y){
  if (!null(x) && !null(y))
    return sym("t");
  else
    return nil;
}

struct object * not(struct object * o){
  if (o == nil)
    return sym("t");
  else
    return nil;
}


struct object * sym(char * name){
  return st_insert(name);
}

struct object * cadr(struct object * o){
  return car(cdr(o));
}

struct object * caddr(struct object * o){
  return car(cdr(cdr(o)));
}

static struct object * cadddr(struct object * o){
  return car(cdr(cdr(cdr(o))));
}

struct object * caar(struct object * o){
  return car(car(o));
}

struct object * caddar(struct object * o){
  return car(cdr(cdr(car(o))));
}

struct object * cadar(struct object * o){
  return car(cdr(car(o)));
}

struct object * append(struct object * x, struct object * y){
  if (null(x))
    return y;
  
  return cons(car(x), append(cdr(x), y));
}


struct object * pair(struct object * x, struct object * y) {
  if (null(x) && null(y))
    return nil;
  
  if (!atom_p(x) && !atom_p(y))
    return cons(list(2, car(x), car(y)), pair(cdr(x), cdr(y)));

  fprintf(stderr, "illegal state");
  exit(1);
}

struct object * assoc(struct object * x, struct object * y) {
  if (null(x)) {
    fprintf(stderr, "nil is not a variablename\n");
    exit(EXIT_FAILURE);
  }
  
  if (null(y)) {
    fprintf(stderr, "no variable %s defined\n", st_id_to_name(x->symbol_index));
    exit(1);
  }
  if (eq(caar(y), x))
    return cadar(y);

  return assoc(x, cdr(y));
}

static struct object * evcon(struct object * c, struct object *a);
static struct object * evlis(struct object * m, struct object *a);

struct object * globals = nil;
struct object * globals_end = nil;

struct object * eval(struct object * e, struct object *a){
  if (null(e))
    return nil;
  if (atom_p(e))
    return assoc(e, a);
  if (atom_p(car(e))) {
    if (eq(car(e), sym("eval")))
      return eval(eval(cadr(e), a), append(eval(caddr(e), a), a));

    if (eq(car(e), sym("quote")))
      return cadr(e);

    if (eq(car(e), sym("atom")))
      return atom_p(eval(cadr(e), a));

    if (eq(car(e), sym("eq")))
      return eq(eval(cadr(e), a), eval(caddr(e), a));

    if (eq(car(e), sym("car")))
      return car(eval(cadr(e), a));

    if (eq(car(e), sym("cdr")))
      return cdr(eval(cadr(e), a));
    
    if (eq(car(e), sym("cons")))
      return cons(eval(cadr(e), a), eval(caddr(e), a));

    if (eq(car(e), sym("cond")))
      return evcon(cdr(e), a);

    if (eq(car(e), sym("print"))) {
      print(eval(cadr(e), a));
      puts("");
      return nil;
    }

    if (eq(car(e), sym("defun"))) {
      struct object *name = cadr(e);
      struct object *params = caddr(e);
      struct object *body = cadddr(e);
      struct object *pair = cons(list(2, name, list(3, sym("lambda"), params, body)), nil);
      globals_end->data->cdr = pair;
      globals_end = pair;
      return eval(name, a);
    }

    /*    if (eq(car(e), sym("list"))) {
      
	  }*/

    if (eq(car(e), sym("append")))
      return append(eval(cadr(e), a), eval(caddr(e), a));
    
    return eval(cons(assoc(car(e), a), cdr(e)), a);
  }
  
  if (eq(caar(e), sym("label")))
    return eval(cons(caddar(e), cdr(e)), cons(list(2, cadar(e), car(e)), a));

  if (eq(caar(e), sym("lambda")))
    return eval(caddar(e), append(pair(cadar(e), evlis(cdr(e), a)), a));

  fprintf(stderr, "reached end of eval. probably a syntax error");
  exit(1);
}

struct object * evcon(struct object * c, struct object *a){
  if (eval(caar(c), a))
    return eval(cadar(c), a);
  
  return evcon(cdr(c), a);
}

struct object * evlis(struct object * m, struct object *a){
  if (null(m))
    return nil;
  
  return cons(eval(car(m), a), evlis(cdr(m), a));
}
