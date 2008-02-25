#include <stdbool.h>
#include <stdio.h>
#include "base.h"
#include "symbol_table.h"

/*
lisp_program -> atom|list
list -> "(" list_body ")"
list_body -> (atom|list)+
 */

static char *sym;

static bool is_o(){
  return sym[0]=='(';
}

static bool is_c(){
  return sym[0]==')';
}

static bool is_sym(){
  return !(is_o() || is_c());
}

static void match(char * s){
  if (strcmp(s, sym)==0) {
    sym = yylex();
  }
  else {
    fprintf(stderr, "parse error\n");
    exit(1);
  }
}

static struct object * atom_or_list();

static struct object * list_body(){
  if (is_c())
    return nil;
  return cons(atom_or_list(), list_body());
}

static struct object * parse_list() {
  match("(");
  struct object * o;
  o = list_body();
  match(")");
  return o;
}

static struct object * parse_atom() {
  static struct object * o;
  o = st_insert(sym);
  sym = yylex();
  return o;
}

static struct object * atom_or_list(){
  if (is_sym())
    return parse_atom();
  else if (is_o())
    return parse_list();
  else {
    fprintf(stderr, "parse error\n");
    exit(1);
  }
}

static struct object * lisp_program(){
  return atom_or_list();
}

struct object * parse(){
  sym = (char *)yylex();
  return lisp_program();
}

