#ifndef __PARSER_H
#define __PARSER_H
#include "base.h"

enum symbol_types { SYMBOL=1, OPENING_BRACE, CLOSING_BRACE };

object * parse_string(char *s);

#endif
