#include "renderer.h"
#include <ncurses.h>

// commands.h abstraction with command hash map 
KeyLayout keys;
Renderer renderer;
Command commands;

void initialize_commands() {
	keys.map_key_layout();	
}

void main_loop() {
	while (true) {
		char ch = getch();
		keys.execute_command(std::to_string(ch));
		
		if (commands.edit_mode) std::string command = commands.get_command();
	}
}

int main() {
	// Window setup for ncurses
	initscr();
	keypad(stdscr, TRUE);
	refresh();
	
	main_loop();

	endwin();
	
    return 0;
}

