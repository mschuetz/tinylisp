#ifndef __PARSER_H
#define __PARSER_H

enum symbol_types { SYMBOL, OPENING_BRACE, CLOSING_BRACE };

struct object * parse_string(char *s);

#endif
