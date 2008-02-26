#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "base.h"
#include "symbol_table.h"


/*
lisp_program -> atom|list
list -> "(" list_body ")"
list_body -> (atom|list)+
 */

static char *_sym;

static bool is_o(){
  return _sym[0]=='(';
}

static bool is_c(){
  return _sym[0]==')';
}

static bool is_sym(){
  return !(is_o() || is_c());
}

static void match(char * s){
  if (strcmp(s, _sym)==0) {
    _sym = (char *)yylex();
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
  if (strcmp(_sym, "nil")==0)
    o = nil;
  else
    o = st_insert(_sym);
  _sym = (char *)yylex();
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
  _sym = (char *)yylex();
  return lisp_program();
}

extern FILE * yyin;

struct object * parse_string(char *s){
  struct object *o;
  char * fn = "/tmp/tinylisp-123";
  FILE * f = fopen(fn, "w");
  fwrite(s, strlen(s), 1, f);
  fclose(f);
  FILE * tmp = yyin;
  yyin = fopen(fn, "r");
  o = parse();
  fclose(yyin);
  yyin = tmp;
  return o;
}
