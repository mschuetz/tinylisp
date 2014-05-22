#ifndef __PARSER_H
#define __PARSER_H

enum symbol_types { SYMBOL, OPENING_BRACE, CLOSING_BRACE };

object * parse_string(char *s);

#endif
