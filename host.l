%{

#include <stdio.h>
#include <string.h>
#include "host.tab.h"
void showError();
%}

DIGIT     ([0-9])
alpha       ([a-zA-Z_])+


%%


[ \t\r\n]+      {
 /* skip whitespaces */
                }
"#".*           {
    /* skip comments */
                }

{alpha}         {
                      if (!strncmp(yytext, "node", sizeof("node"))) {
                          return (NODE);
                      }
                      else if (!strncmp(yytext, "param", sizeof("param"))) {
                          return (PARAM);
                      }
                      else if (!strncmp(yytext, "port", sizeof("port"))) {
                          return (PORT);
                      }
                      else if(!strncmp(yytext, "enum", sizeof("enum"))) {
                          return (ENUM);
                      }
                      sscanf(yytext, "%s", yylval.name);
                      return (STRING);
                }

{DIGIT}+        {
                      yylval.number = atof(yytext);
                      return NUM;
                }

":"             { return COLON;
                }
"="             { return EQUAL;
                }
"{"             { return OPEN_CURLY_BRACKET;
                }
"}"             { return CLOSE_CURLY_BRACKET;
                }
"["             { return OPEN_SQUARE_BRACKET;
                }
"]"             { return CLOSE_SQUARE_BRACKET;
                }
"("             {
                  return OPEN_BRACKET;
                }
")"             {
                  return CLOSE_BRACKET;
                }
"/"             {
                  return SLASH;
                }
"-"             {
                  return DASH;
                }
"\""            {
                  return QUOTE;
                }
"."             {
                  return DOT;
                }
","             {
                  return COMMA;
                }
