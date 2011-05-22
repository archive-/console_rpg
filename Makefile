all: roguelike

roguelike: main.c
	gcc -Wall -g -o roguelike main.c -ltcod

clean:
	rm -rf roguelike
