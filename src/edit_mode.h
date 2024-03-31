#include <ios>
#include <ncurses.h>
#include <stdexcept>
#include <string>
#include "key_layout.h"

class EditMode {
	private: void add_to_header(std::string &header, int &x, char &ch) {
				std::string s = std::to_string(ch);
				header.insert(x, s);	
			 }

	public: std::string get_current_header() {
				int x, y;
				getyx(stdscr, y, x);

				return Command::get_header(y);
			}

	private: bool move_in_range(std::string &header, int &x, int &y) {
				if (y < 0) return false;
				if (y > 9 or y > Command::get_headers_size()) return false;
				if (x < 2 or x > header.length()) return false;

				return true;
			 }

	public: void initialize_command(char &ch, std::string header) {
		int x, y;
		getyx(stdscr, y, x);

		if (ch < 2 or ch > 5) {
			add_to_header(header, x, ch);
		} 
		
		// Just the movement keys left	
		switch (ch) {
			case ARROW_LEFT:
				x--;
				break;
			case ARROW_RIGHT:
				x++;
				break;
			case ARROW_UP:
				y--;
				break;
			case ARROW_DOWN:
				y++;
				break;
		}
		
		if (move_in_range(header, x, y)) {
			try {
				move(y, x);
			} catch (...) {
				return;
			}
		}

		refresh();
	}
};
