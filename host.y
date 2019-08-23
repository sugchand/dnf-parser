%{
#include <stdio.h>
#include "host_helper.h"

%}

%token COLON EQUAL OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET OPEN_SQUARE_BRACKET
       NUM STRING ENUM CLOSE_SQUARE_BRACKET SLASH DASH QUOTE NODE PARAM PORT
       DOT COMMA OPEN_BRACKET CLOSE_BRACKET

%union{
        char str[1000];
        int number;
}

%type <str> STRING
%type <number> NUM

%start hostApp

%%

hostApp:
    node paramSet portSet

node:
    NODE COLON string
    {
        hostConfigAdd("node", $<str>3, str_param);
    }

paramSet:
          paramSet paramEntry
        | paramEntry

paramEntry:
        PARAM COLON paramEntryName paramValueSet

paramEntryName:
              string

paramValueSet:
             open_curly_bracket paramValues close_curly_bracket

open_curly_bracket:
                    OPEN_CURLY_BRACKET
                    {
                        // update the tree level
                        increment_dnf_obj_level();
                    }

close_curly_bracket:
                    CLOSE_CURLY_BRACKET
                    {
                        // update the tree level
                        decrement_dnf_obj_level();
                    }

paramValues:
             paramName COLON paramValue
             {
             }
           | paramValues COMMA paramName COLON paramValue
             {
             }

paramName:
          string

paramValue:
            string
            {
            }
          | QUOTE string QUOTE
            {
            }
          | NUM
            {
            }
          | ENUM OPEN_BRACKET enumSet CLOSE_BRACKET
            {
            }

enumSet:
        enumValue
        | enumSet COMMA enumValue
          {
              snprintf_safe($<str>$, 1000, "%s,%s", $<str>1,$<str>3);
          }

enumValue:
         string

portSet:
       PORT COLON string OPEN_SQUARE_BRACKET portIndex CLOSE_SQUARE_BRACKET
       {
           char portVal[1000];
           snprintf_safe(portVal, 1000, "%s%s", $<str>3, $<str>5);
           hostConfigAdd("port", portVal, str_param);
       }

portIndex:
           NUM
           {
               snprintf_safe($<str>$, 1000, "%d", $<number>1);
           }
         | NUM COLON NUM
           {
               snprintf_safe($<str>$, 1000, "%d-%d", $<number>1, $<number>3);
           }

string:
        STRING
        {
            snprintf_safe($<str>$, 1000, "%s", $<str>1);
        }
      | STRING NUM
        {
            snprintf_safe($<str>$, 1000, "%s%d", $<str>1,$<number>2);
        }
      | string DASH STRING
        {
            snprintf_safe($<str>$, 1000, "%s-%s", $<str>1,$<str>3);
        }
      | string DASH STRING NUM
        {
            snprintf_safe($<str>$, 1000, "%s-%s%d", $<str>1,$<str>3, $<number>4);
        }
      | string STRING
        {
            snprintf_safe($<str>$, 1000, "%s,%s", $<str>1,$<str>2);
        }
%%

