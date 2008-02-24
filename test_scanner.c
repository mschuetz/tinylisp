int main(){
  char * sym;
  while (sym = (char *)yylex())
    printf("got >%s<\n", sym);
}
