%{

%}

digit   [0-9]
blank   [ \t]
name1   [A-Za-z]
name2   [A-Za-z_0-9]

%%

{name1}{name2}*         { return (int)yytext; }


"(" {return (int)yytext;}
")" {return (int)yytext;}

"\n"                    { }
			
{blank}+                ;
%%
