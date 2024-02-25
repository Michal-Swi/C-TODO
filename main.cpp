#include <ncurses.h>
#include "commands.h"

// LOGIC
void commandHandler(char commandKey) {
	try {
		commands[commandKey]->initializeCommand();
	} catch (...) {
		return;
	}
	
	return;
}

void mainLoop() {
	while (true) {
		char ch = getch();
		if (ch == 'q') return; // Ending the loop in commands.h kills main.cpp in some werid way

		commandHandler(ch);
		refresh();
	}
}

int main() {
	initscr();

	mvprintw(10, 10, "LOADING...");
	refresh();
	
	initializeCommands();
	
	// Everything loaded 
	clear();
	
	mainLoop();

	endwin();

	return 0;
}

