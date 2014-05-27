%{

#include "parser.h"

%}

digit   [0-9]
blank   [ \t]
name1   [A-Za-z]
name2   [A-Za-z_0-9]

%%

{name1}{name2}*         { return SYMBOL; }


"(" {return OPENING_BRACE;}
")" {return CLOSING_BRACE;}

"\n"                    { }
			
{blank}+                ;
%%
