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
  fprintf(stderr, "not a list (in car)\n");
  print(o);
  exit(1);
}

struct object * cdr(struct object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return ((struct cons_cell *)o->data)->cdr;
    else
      return nil;
  }
  fprintf(stderr, "not a list (in cdr)\n");
  exit(1);
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
  print(car(o));
  printf(" . ");
  print(cdr(o));
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
}

struct object * assoc(struct object * x, struct object * y) {
  if (null(x)) {
    fprintf(stderr, "nil is not a variablename\n");
    exit(1);
  }
  
  if (null(y)) {
    fprintf(stderr, "no variable %s defined\n", st_id_to_name((atom)x->data));
    exit(1);
  }
  if (eq(caar(y), x))
    return cadar(y);

  return assoc(x, cdr(y));
}
struct object * evcon(struct object * c, struct object *a);
struct object * evlis(struct object * m, struct object *a);

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
      return evcon(cdr(e),a);
    
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



