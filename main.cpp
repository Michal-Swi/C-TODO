#include <ncurses.h>

int main() {
	initscr();

	printw("Test");

	refresh();
	
	return 0;
}

