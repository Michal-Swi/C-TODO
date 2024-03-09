#include "renderer.h"
#include <ncurses.h>

// commands.h header abstraction
Command commands;

void main_loop() {
	Renderer renderer;
	renderer.render_user_text();	
	refresh();
	while (1);
}

int main() {

	// Window setup for ncurses
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	refresh();
	
	// Setting up the commands
	commands.key_layout["q"] = new ExitCommand();

	main_loop();

	endwin();

    return 0;
}

