#include "renderer.h"
#include <ncurses.h>

EditMode edit_mode;
KeyLayout keys;
Renderer renderer;
Command commands;

void main_loop() {
	while (true) {
		renderer.render_all_headers
			(headers.get_headers(), Command::get_current_command());

		if (commands.edit_mode) {
			char ch = getch();
			if (ch == ':') { keys.execute_command(":"); continue; }
				 
			edit_mode.initialize_command(ch); 
		} else {
			Command::set_current_command(commands.get_command());
			keys.execute_command(Command::get_current_command());
		}
	}
}

int main() {
	// Window setup for ncurses
	initscr();
	keypad(stdscr, TRUE);
	refresh();
	noecho();
	CompletionLevels::fill_completion_levels();

	main_loop();

	endwin();

    return 0;
}
