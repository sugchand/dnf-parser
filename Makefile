default:
	clear
	flex -dl host.l
	bison -tdv host.y 
	gcc -o hostParse host.tab.c lex.yy.c -lfl
