#include <csignal>
#include <ios>
#include <ncurses.h>
#include <stdexcept>
#include <string>
#include "key_layout.h"

class EditMode {
	private: std::string get_current_header_name() {
				int x, y;
				getyx(stdscr, y, x);

				return headers.get_header_flat(y).get_header_name();
			}
	
	private: void delete_character(int &y, int &x, 
			  std::string &header) {
				if (x == 2) return;

				int i = 0;
				std::string new_header_name;

				for (; i < x - 3; i++) new_header_name += header[i];
				i++;

				for (; i < headers.get_header_flat(y).get_header_name().length(); i++, new_header_name += header[i]);

				headers.change_header_name(headers.get_header_flat(y).get_header_name(), new_header_name);

				move(y, x - 1);
			 }	

	private: void add_character
			 (int &y, int &x, char ch, std::string &previous_header_name) {
				 std::string new_header_name;

				int i = 0;

				for (; i < x - 2; i++) new_header_name += previous_header_name[i];
				new_header_name += ch;
				
				for (; i < headers.get_header_flat(y).get_header_name().length(); i++) { 
					new_header_name += previous_header_name[i];
				}
				
				move(y, x + 1);

				headers.change_header_name(headers.get_header_flat(y).get_header_name(), new_header_name);
			 }

	private: void different_action(int &y, int &x, char &ch) {
				std::string previous_header_name = headers.get_header_flat(y).get_header_name();
				
				if (ch == ENTER_KEY) return; // Tf you trying to do
				if (ch == BACKSPACE_KEY) {
					delete_character(y, x, previous_header_name);
				} else add_character(y, x, ch, previous_header_name);		
			}
	
	private: bool move_in_y_direction(int &y, int &previous_y) {
				return y != previous_y;
			 }

	private: bool move_in_range(int &x, int &y) {
				if (y < 0) return false;
				if (y > 9 or y >= headers.get_header_flat(y).get_header_name().length()) return false;
				if (x == 2) return true;
				if (x - 2 > headers.get_header_flat(y).get_header_name().length()) return false;

				return true;
			 }

	public: void initialize_command(char &ch) {
		int x, y;
		getyx(stdscr, y, x);

		if (ch < 2 or ch > 5) {
			different_action(y, x, ch);
			return;
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
		if (move_in_y_direction(y, previous_y)) {
			if (x > headers.get_header_flat(y).get_header_name().length()) {
				x = headers.get_header_flat(y).get_header_name().length() + 1;
			}
		} 

		if (move_in_range(x, y)) {
			try {
				move(y, x);
			} catch (...) {
				return;
			}
		}
	}
};
