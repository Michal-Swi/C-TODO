#include <csignal>
#include <ios>
#include <ncurses.h>
#include <stdexcept>
#include <string>
#include "key_layout.h"

class EditMode {
	private: std::string get_current_header() {
				int x, y;
				getyx(stdscr, y, x);

				return Command::get_header(y);
			}

	private: void different_action(int &x, char &ch) {
				
			 }
	
	private: bool move_in_y_direction(int &y, int &previous_y) {
				return y != previous_y;
			 }

	private: bool move_in_range(int &x, int &y) {
				if (y < 0) return false;
				if (y > 9 or y >= Command::get_headers_size()) return false;
				if (x - 2 >= Command::get_header(y).length()) return false;

				return true;
			 }

	public: void initialize_command(char &ch) {
		int x, y;
		getyx(stdscr, y, x);

		if (ch < 2 or ch > 5) {
			different_action(x, ch);
		} 
		
		int previous_x = x, previous_y = y;

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
		
		// Moves in y direction are legal even when
		// the length of two headers is different
		if (move_in_y_direction(y, previous_y));
	}
};
