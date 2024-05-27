#include "modes.h"
#include <ncurses.h>

EditMode edit_mode;
KeyLayout keys;
Command commands;
NormalMode normal_mode;

void main_loop() {
	while (true) {
		renderer.render_headers(Headers::get_headers_flat(), Command::get_current_command());	
		if (!Command::edit_mode) {
			normal_mode.initialize_command();
		} else {
			edit_mode.initialize_command();
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
