#include "modes.h"
#include <ncurses.h>
#include <vector>

EditMode edit_mode;
Command commands;
NormalMode normal_mode;

void main_loop() {
	while (true) {
		std::vector<HeaderFlat> headers_to_render = Headers::get_headers_flat();
		
		renderer.render_headers(headers_to_render, Command::get_current_command());	

		if (headers_to_render.empty()) {
			renderer.remove_last_line();

			std::string initial_header_name = commands.get_header_name();
			std::string initial_header_path = commands.create_path_to_self(initial_header_name, ".");

			headers.add_initial_header(initial_header_name, initial_header_path);
		}
		
		if (!Command::edit_mode) {
			normal_mode.initialize_command();
		} else {
			edit_mode.initialize_command();
		}
	}
}

int main(int argc, char **argv) {
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
