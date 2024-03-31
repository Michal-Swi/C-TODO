#include "renderer.h"
#include <ncurses.h>

EditMode edit_mode;
KeyLayout keys;
Renderer renderer;
Command commands;

void main_loop() {
	while (true) {
		renderer.render_headers(commands.get_headers());
		if (commands.edit_mode) {
			char ch = getch();
			edit_mode.initialize_command(ch, edit_mode.get_current_header());
		} else {
			std::string command = commands.get_command();
			keys.execute_command(command);
		}
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

