%{

%}

digit   [0-9]
blank   [ \t]
name1   [A-Za-z]
name2   [A-Za-z_0-9]

%%

{name1}{name2}*         { return (char *)yytext; }


"(" {return (char *)yytext;}
")" {return (char *)yytext;}

"\n"                    { }
			
{blank}+                ;
%%
