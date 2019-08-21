%{
#include <stdio.h>
#include "host.h"

int yylex();
int yyerror(char *s);

%}

%token COLON EQUAL OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET OPEN_SQUARE_BRACKET
       NUM STRING ENUM CLOSE_SQUARE_BRACKET SLASH DASH QUOTE NODE PARAM PORT
       DOT COMMA OPEN_BRACKET CLOSE_BRACKET

%type <name> STRING
%type <number> NUM

%union{
        char name[1000];
        int number;
}

%%

hostApp:
    node paramSet portSet

node:
    NODE COLON string

paramSet:
          paramSet paramEntry
        | paramEntry

paramEntry:
        PARAM COLON paramEntryName paramValueSet

paramEntryName:
              string

paramValueSet:
             OPEN_CURLY_BRACKET paramValues CLOSE_CURLY_BRACKET

paramValues:
             paramName COLON paramValue
           | paramValues COMMA paramName COLON paramValue

paramName:
          string

paramValue:
            string
          | QUOTE string QUOTE
          | NUM
          | ENUM OPEN_BRACKET enumSet CLOSE_BRACKET

enumSet:
        enumValue
        | enumSet COMMA enumValue

enumValue:
         string

portSet:
       PORT COLON string OPEN_SQUARE_BRACKET portIndex CLOSE_SQUARE_BRACKET

portIndex:
           NUM
         | NUM COLON NUM

string:
        STRING
      | STRING NUM
      | string DASH STRING
      | string DASH STRING NUM
      | string STRING
%%

int yyerror(char *s)
{
    extern int yylineno;
    printf("Error on line : %d,  %s\n", yylineno, s);
    return 0;
}

int main(int argc, char *argv[])
{
    extern FILE *yyin;
/* Enable it to debug the parsing of tokens,
 * make sure bison command is provided with -t.
 * .output file provide insights on parser stages
 */
//#ifdef YYDEBUG
//      yydebug = 1;
//#endif

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            perror(argv[1]);
            return 1;
        }
    }
    else {
        printf("Reading the input from stdin\n");
    }
    if (!yyparse()) {
        printf("Successfully completed the token parsing\n");
    }
    printHostConfig();
    return 0;
}
