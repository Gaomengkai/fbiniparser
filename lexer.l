%option noyywrap
%option never-interactive
%option nounistd

%{
#include <string>
#include "ast.hpp"
#include "parser.tab.hpp"
%}
%option yylineno
SingleLineComment1 ";".*\n$
SingleLineComment2 "#".*\n$
%%
{SingleLineComment1}|{SingleLineComment2}             /* 忽略注释行 */{}
\[.*\]            /* 匹配节（section） */{yylval.str = new std::string(yytext); return SECTION;}
[^= \r\n\t]+[ \t]*=[ \t]*[^=\r\n\t]+ {yylval.str = new std::string(yytext); return KVPAIR;}
[ \t\n]+            /* 忽略空格和制表符 */{}
.               /* 匹配其他字符 */{}
%%

