#include <ios>
#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "edit_mode.h"

class Renderer {
	public: void render_headers(std::vector<std::string> tasks) {
		int x, y;
		getyx(stdscr, y, x);

		if (tasks.empty()) {
			printw("NO TASKS");
			return;
		}
			
		clear();
		
		for (int i = 0; i <std::min(10, Command::get_headers_size()) ; i++) {
			tasks[i] = std::to_string(i + 1) + '.' + tasks[i];
			mvprintw(i, 0, tasks[i].c_str());
			refresh();
		}
		
		if (Command::get_headers_size() > 10) printw("\nPress n to render next page");
		
		move(y, x);
		refresh();
	}	
	
	private: std::string get_text() {
		std::string text;
		
		char ch;
		while (ch != ENTER_KEY) {
			ch = getch();
			if (ch == ENTER_KEY and text.empty()) ch = 'p'; // Something different 
															// from ENTER_KEY so that
															// the loop continues
			else if (ch != ENTER_KEY) {
				text += ch;
				addch(ch);
			}
		}

		int y, x;
		getyx(stdscr, y, x);
		
		move(y, 0); // Moves to the start of the line to clear it
		clrtoeol(); // Clears the line from addch();
		refresh();

		return text;
	}

	// Outputs text written by user
	public: void render_user_text() {
		std::string user_text = get_text();
		
		std::fstream headers;
		headers.open("../data/headers.txt", std::ios_base::app);

		printw(user_text.c_str());
	}
};

