.PHONY: default
default: bison lex

bison:
	bison -d -o parser.tab.cpp parser.y

lex:
	flex -o lex.yy.cpp lexer.l


all: bison lex
	clang++ -DYYDEBUG=1 -std=c++17 -g -o main main.cpp parser.tab.cpp lex.yy.cpp

run: all
	./main

.PHONY: clean
clean:
	rm -f parser.tab.* lex.yy.cpp y.output main *.out stack.hh

