#include "renderer.h"
#include <ncurses.h>

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

	protected: Renderer new_renderer;

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
		int y = 0;
		move(y, 0);
		
		headers.change_colored_flat(true, y);
		new_renderer.render_headers(headers.get_headers_flat(), Command::get_current_command());
		
		while (true) {
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
			}
				
			new_renderer.render_headers(headers.get_headers_flat(), Command::get_current_command()); 
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

	public: void initialize_command() override {
				curs_set(1); // Makes cursor visible
				
				int y = 0, x = 0;
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
					}
				}
			}
};
