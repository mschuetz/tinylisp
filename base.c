#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <readline/readline.h>
#include "base.h"
#include "symbol_table.h"

object * car(object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return o->data->car;
    else
      return nil;
  }
  check(false, "argument not a list");
}

object * cdr(object * o){
  if (!atom_p(o)) {
    if (o!=nil)
      return o->data->cdr;
    else
      return nil;
  }
  check(false, "argument not a list");
}

object * atom_p(object * o){
  if (o!=nil && o->atom_p)
    return sym("t");
  else
    return nil;
}

object * eq(object * o1, object * o2){
  if ((atom_p(o1) && atom_p(o2) && o1==o2) || (o1==o2 && o2==nil))
    return sym("t");
  else
    return nil;
}

object * list(int len, ...) {
  va_list ap;
  object * start = nil;
  object * cur = nil;
  object * o = nil;
  int i;
  start = cons(nil, nil);
  cur = start;
  va_start(ap, len);
  for (i=0;i<len;i++){
    o = va_arg(ap, object *);
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

object * cons(object * o1, object * o2){
  object * o = (object *) malloc(sizeof(object));
  o->atom_p = false;
  o->function_p = false;
  cons_cell * cc = (cons_cell *) malloc(sizeof(cons_cell));
  o->data = cc;
  cc->car = o1;
  cc->cdr = o2;
  return o;
}

object * quote(object * o){
  return o;
}

static void print_cons(object * o);
static void print_atom(atom a);

object * print(object * o){
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

static void print_cons(object * o){
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

object * null(object * o){
  return eq(o, nil);
}

object * and(object * x, object * y){
  if (!null(x) && !null(y))
    return sym("t");
  else
    return nil;
}

object * not(object * o){
  if (o == nil)
    return sym("t");
  else
    return nil;
}


object * sym(const char * name){
  return st_insert(name);
}

object * cadr(object * o){
  return car(cdr(o));
}

object * caddr(object * o){
  return car(cdr(cdr(o)));
}

static object * cadddr(object * o){
  return car(cdr(cdr(cdr(o))));
}

object * caar(object * o){
  return car(car(o));
}

object * caddar(object * o){
  return car(cdr(cdr(car(o))));
}

object * cadar(object * o){
  return car(cdr(car(o)));
}

object * append(object * x, object * y){
  if (null(x))
    return y;
  
  return cons(car(x), append(cdr(x), y));
}


object * pair(object * x, object * y) {
  if (null(x) && null(y))
    return nil;
  
  if (!atom_p(x) && !atom_p(y))
    return cons(list(2, car(x), car(y)), pair(cdr(x), cdr(y)));

  fprintf(stderr, "illegal state");
  exit(1);
}

object * assoc(object * x, object * y) {
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

static object * evcon(object * c, object *a);
static object * evlis(object * m, object *a);

object * globals = nil;
object * globals_end = nil;

object * eval(object * e, object *a){
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
      object *name = cadr(e);
      object *params = caddr(e);
      object *body = cadddr(e);
      object *pair = cons(list(2, name, list(3, sym("lambda"), params, body)), nil);
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

object * evcon(object * c, object *a){
  if (eval(caar(c), a))
    return eval(cadar(c), a);
  
  return evcon(cdr(c), a);
}

object * evlis(object * m, object *a){
  if (null(m))
    return nil;
  
  return cons(eval(car(m), a), evlis(cdr(m), a));
}
