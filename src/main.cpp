#include <ncurses.h>
#include <string>
#include "renderer.h"

void mainLoop() {
	while (true) {
		drawScreen();	
	}
}

int main() {
	initscr();
	keypad(stdscr, true);

	clear();
	
	mainLoop();

	endwin();

    return 0;
}

