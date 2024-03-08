#include "renderer.h"

// commands.h header abstraction
Command commands;

void main_loop() {
	Renderer renderer;
	
	std::string current_command;
	while (true) {
		refresh();
		char ch = getch();

		if (ch != ENTER_KEY) {
			current_command += ch;
			continue;
		}
		
		try {
			commands.key_layout[current_command];	
		} catch (...) {
			mvprintw(80, 0, ("No command named: " + current_command).c_str());
		}
	}
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

