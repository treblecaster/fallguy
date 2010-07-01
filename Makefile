all: go

go: main.c
	gcc -o go -lncurses main.c
