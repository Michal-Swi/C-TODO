#include "renderer.h"
#include <ncurses.h>

// Header abstraction import

KeyLayout keys;
Renderer renderer;
Command commands;

void main_loop() {
	while (true) {
		renderer.render_headers(commands.get_headers());
		std::string command = commands.get_command();
		keys.execute_command(command);
	}
}

int main() {
	// Window setup for ncurses
	initscr();
	keypad(stdscr, TRUE);
	refresh();
	noecho();
	commands.read_headers();
	
	main_loop();

	endwin();
	

    return 0;
}

