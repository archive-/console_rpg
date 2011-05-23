all: roguelike

roguelike: main.c
	gcc -Wall -g -o roguelike main.c txtfield_c.c -ltcod

clean:
	rm -rf roguelike
