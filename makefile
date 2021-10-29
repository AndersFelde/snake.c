all:
	gcc main.c lib/snake.c lib/board.c lib/tools.c -o snake -lncurses

clean:
	rm ./snake
