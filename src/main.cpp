#include "renderer.h"
#include <cstddef>
#include <ncurses.h>

EditMode edit_mode;
KeyLayout keys;
Renderer renderer;
Command commands;

void main_loop() {
	while (true) {
		renderer.render_headers(Headers::get_headers_flat(), Command::get_current_command());	

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
	initscr();
	keypad(stdscr, TRUE);
	refresh();
	noecho();
	CompletionLevels::fill_completion_levels();
	
	start_color();
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK); 
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	
	attron(COLOR_PAIR(1));

	main_loop();
	
	endwin();

    return 0;
}
