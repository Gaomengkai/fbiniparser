%define parse.error verbose
%locations
%{
#include <iostream>
#include <string>
#include "ast.hpp"
using namespace std;

extern int yylex();
extern void yyerror(const char *);
extern int yylineno;

std::unique_ptr<AST> root;

%}

%union {
    std::string* str;
    AST* rootAST;
    SectionAST*    sectionAST;
    SectionItemAST* sectionItem;
    SectionItemsAST* sectionItems;
    SectionListAST* sectionList;
}

%type <rootAST> root
%type <sectionAST> section

%type <sectionItem> section_item
%type <sectionItems> section_items
%type <sectionList> section_list

%token <str> SECTION
%token <str> STRING
%token <str> KVPAIR
%token EQUALS
%token COMMENT
%token OTHERS
%token SPLITER

%%
root: section_list {
    $$=new AST();
    $$->add(std::unique_ptr<SectionListAST>($1));
    root = std::unique_ptr<AST>($$);
}
    | /* 空文件 */
    {
        $$=new AST();
        root = std::unique_ptr<AST>($$);
    }
    ;

section_list: section_list section{
    $$=$1;
    $$->add(std::unique_ptr<SectionAST>($2));
}
    | section{
        $$=new SectionListAST();
        $$->add(std::unique_ptr<SectionAST>($1));
    }
    ;

section: SECTION section_items {
    // SECTION contains [ and ]
    // we must remove [ and ] from $1
    std::string* sectionName = new std::string($1->substr(1, $1->length()-2));
    $$=new SectionAST(sectionName);
    $$->add(std::unique_ptr<SectionItemsAST>($2));
}
    ;

section_items: section_items section_item {
    $$=$1;
    $$->add(std::unique_ptr<SectionItemAST>($2));
}
    | section_item {
        // 新建一个SectionItemsAST
        $$=new SectionItemsAST();
        // 将section_item加入到SectionItemsAST中
        $$->add(std::unique_ptr<SectionItemAST>($1));
}
    ;

section_item: KVPAIR {
    $$ = new SectionItemAST($1);
}
    ;

%%


void yyerror(const char* fmt) {
    printf("%d ", yylloc.first_line);
    printf("%s\n", fmt);
}