#include "key_layout.h"
#include <fstream>
#include <ncurses.h>
#include <string>

enum Directions {
	Left = 0,
	Right = 1,
	Down = 2,
	Up = 3,
};

class Modes : public Command {
	protected: int get_amount_of_leading_spaces(const int &y) {
				return headers.get_depth_flat(y) * 4;  
			 }	
	
	private: void fix_boundries(int &x, const Directions &direction) {
				assert(direction == Directions::Up or direction == Directions::Down);

				int target_y;
				getyx(stdscr, target_y, x);
				
				if (direction == Directions::Up) target_y--;
				else if (direction == Directions::Down) target_y++;

				int leading_spaces = get_amount_of_leading_spaces(target_y);
				if (x <= leading_spaces) {
					x = leading_spaces;
				} else if (x > leading_spaces + headers.get_header_flat(target_y).get_header_name().length()) {
					x = leading_spaces + headers.get_header_flat(target_y).get_header_name().length();
				}
			 }

	protected: void move_up(const bool &change_color, int &y, const bool check_boundries = false) {
				if (y == 0) {
					return;
				}

				if (change_color) {
					headers.change_colored_flat(false, y);	
				}
				
				int x = 0;
				if (check_boundries) {
					fix_boundries(x, Directions::Up);
				}

				y--;
				move(y, x);
				
				if (change_color) headers.change_colored_flat(true, y);
			   }
	
	protected: void move_down(const bool &change_color, int &y, const bool check_boundries = false) {
				if (y + 1 >= Headers::get_headers_flat().size()) return;
					
				if (change_color) headers.change_colored_flat(false, y);
				
				int x = 0;
				if (check_boundries) {
					fix_boundries(x, Directions::Down);
				}

				y++;
				move(y, x);

				if (change_color) headers.change_colored_flat(true, y);
			   }
};

class NormalMode : Modes {
	public: void initialize_command() override {
		curs_set(0);
		
		int y, x;
		getyx(stdscr, y, x);
		y = 0;	
	
		move(y, 0);
		
		if (!headers.get_headers_flat().empty()) headers.change_colored_flat(true, y);
		renderer.render_headers(headers.get_headers_flat(), Command::get_current_command());
		
		while (true) {
			getyx(stdscr, y, x);
			char key_pressed = getch();

			switch (key_pressed) {
				case ARROW_UP:
					move_up(true, y);
					break;
				case ARROW_DOWN:
					move_down(true, y);
					break;
				case ':':
					curs_set(1);
					headers.change_colored_flat(false, y);
					Command::edit_mode = true;
					return;
				default:
					std::string command;
					command += key_pressed;
					keys.execute_command(command);	
			}
				
			renderer.render_headers(headers.get_headers_flat(), Command::get_current_command()); 
			Command::set_current_command("");
		}
	}
};

class EditMode : Modes {
	private: bool check_validity_of_move(const Directions &direction) {
				int x, y;
				getyx(stdscr, y, x);

				int leading_spaces = get_amount_of_leading_spaces(y);		
				if (direction == Directions::Left) {
					if (x - 1 <= leading_spaces - 1) return false; 	
				} else if (direction == Directions::Right) {
					if (x >= headers.get_header_flat(y).get_header_name().length() + leading_spaces) return false; 
				}
				
				return true;
			 }

	private: void move_horizontally(const Directions &direction) {
				if (!check_validity_of_move(direction)) return;	
				
				int y, x;
				getyx(stdscr, y, x);

				if (direction == Directions::Right) x++;
				else x--;

				move(y, x);
			 }
	
	private: int get_starting_x(const int &y) {
				int leading_spaces = get_amount_of_leading_spaces(y);
				
				int x = 0;
				if (x < leading_spaces) x = leading_spaces;

				return x;
			 }
	
	private: std::string generate_spaces_from_depth(const int &y) {
				 int spaces_from_depth = get_amount_of_leading_spaces(y);
				 std::string spaces;

				 while (spaces_from_depth--) {
					 spaces += "    ";
				 }

				 return spaces;
			 }
	
	private: std::string delete_spaces_from_depth(const std::string &s, const int &y) {
				int leading_spaces = get_amount_of_leading_spaces(y);
				
				int i = 0;
				std::string formated_string;
				
				for (; i < leading_spaces; i++);
				for (; i < s.length(); i++) formated_string += s[i];

				return formated_string;
			 }

	private: std::string delete_character_from_header_name(const std::string &header_name) {
				int x, y;
				getyx(stdscr, y, x);
				int leading_spaces = get_amount_of_leading_spaces(y);

				if (x <= get_amount_of_leading_spaces(y)) return header_name;

				std::string formated_header_name = header_name;		

				try {
					formated_header_name.erase(x - leading_spaces - 1, 1);	
				} catch (...) {
					return header_name; // that's why header_name is made const 
				}
				
				x--;
				move(y, x);

				return formated_header_name;
			 }
	
	private: std::string add_character_to_header_name(const std::string &header_name, const char &ch_to_add) {
				int x, y;
				getyx(stdscr, y, x);
				int leading_spaces = get_amount_of_leading_spaces(y);

				std::string formated_header_name = header_name;

				try {
					formated_header_name.insert(x - leading_spaces, 1, ch_to_add);
				} catch (...) {
					return header_name;
				}
				
				x++;
				move(y, x);
					
				return formated_header_name;
			 }

	private: void handle_empty_header(Header header) {
				 if (!header.get_header_name().empty()) return;
				 headers.delete_header(header);
				 headers.generate_headers_flat();
			 }

	public: void initialize_command() override {
				curs_set(1);
				
				int y, x;
				getyx(stdscr, y, x);
				x = get_starting_x(y);
				move(y, x);

				while (true) {
					char key_pressed = getch();

					switch(key_pressed) {
						case ARROW_UP:
							move_up(false, y, true);
							break;
						case ARROW_DOWN:
							move_down(false, y, true);
							break;
						case ARROW_LEFT:
							move_horizontally(Directions::Left);
							break;
						case ARROW_RIGHT:
							move_horizontally(Directions::Right);
							break;
						case BACKSPACE_KEY:
							{
							std::string new_header_name = delete_character_from_header_name
								(headers.get_header_flat(y).get_header_name());

							headers.change_header_name_flat(new_header_name, y);
							handle_empty_header(headers.get_header_flat(y));
							break;
							}
						case ':':
							Command::edit_mode = false;
							return;
						default:
							std::string new_header_name = add_character_to_header_name
								(headers.get_header_flat(y).get_header_name(), key_pressed);
							headers.change_header_name_flat(new_header_name, y);
							break;
					}

					renderer.render_headers(headers.get_headers_flat(), Command::get_current_command());
				}
			}
};
