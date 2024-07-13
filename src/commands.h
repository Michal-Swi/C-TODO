#include <algorithm>
#include <fstream>
#include <functional>
#include <ncurses.h>
#include <cstdlib> // For ExitCommand.
#include <ratio>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <tuple>
#include "renderer.h"
#include "key_binds.h"

class Command {
	// For overriding
	public: void virtual read_config() {
				return;
			}

	protected: std::string format_header_name(const std::string &header_name) {
				std::string formated_header_name;
				
				for (const auto &ch : header_name) {
					if (ch == ' ') {
						formated_header_name += '_';
					} else {
						formated_header_name += ch;
					}
				}

				return formated_header_name;
			}
	
	public: std::string create_path_to_self(const std::string &header_name, const std::string &path_to_parent) {
				std::string formated_header_name = format_header_name(header_name);

				if (path_to_parent == ".") return formated_header_name;

				return path_to_parent + '.' + formated_header_name;
			 }


	public: void add_character_to_header
			(std::string &header, char ch, bool &move_to_previous_position) {
				int x, y;
				getyx(stdscr, y, x);
				
				if (x == header.length()) {
					header += ch;
					move_to_previous_position = false;
					return;
				}

				try {
					move_to_previous_position = true;

					std::string s;
					s += ch;

					header.insert(x, s);
				} catch (...) {
					return;
				}
			}

	//Deletes a character left from the current mouse position
	public: void delete_character(std::string &s) {
				int x, y;
				getyx(stdscr, y, x);

				try {
					s.erase(x - 1, 1);
				} catch (...) {
					return;
				}

				move(y, x - 1);
			}

	public: std::string get_header_name() {
			std::string header_name;

			curs_set(1);

			int x, y;
			getyx(stdscr, y, x);
			
			int max_x, max_y;
			getmaxyx(stdscr, max_y, max_x);
			max_x = 0;

			move(max_y - 1, max_x);

			char key_pressed;
			while (key_pressed != ENTER_KEY) {
				key_pressed = getch();

				switch (key_pressed) {
					case ENTER_KEY:
						curs_set(0);
						return header_name;
					case BACKSPACE_KEY:
						if (header_name.empty()) break;
						delete_character(header_name);
						max_x--;
						break;
					case ARROW_LEFT:
						if (max_x == 0) break;
						max_x--;
						break;
					case ARROW_RIGHT:
						if (max_x >= header_name.length()) break;
						max_x++;
						break;
					default:
						header_name += key_pressed;
						max_x++;
						
				}

				move(max_y - 1, 0);
				renderer.render_headers(headers.get_headers_flat(), header_name);
				move(max_y - 1, max_x);
			}
		 }

	public: ~Command() {}

	public: static bool edit_mode;

	private: static std::string current_command;
	
	public: static std::string get_current_command() {
				return current_command;
			}
	
	public: static void set_current_command(const std::string &new_current_command) {
				current_command = new_current_command;
			}
	
	public: static void add_to_current_command(const char &ch) {
				current_command += ch;
			}

	public: virtual void initialize_command() {
			return; // Method for overriding.
		}
};

bool Command::edit_mode = false;
std::string Command::current_command = "";

class ExitCommand : public Command {
	private: std::unordered_map<char, std::function<void()>>
				exit_command_functions;

	public: void read_config() override {
				exit_command_functions[ExitBind::force] = 
					std::bind(&ExitCommand::force, this);

				exit_command_functions[ExitBind::save] =
					std::bind(&ExitCommand::save, this);
			}

	public: void initialize_command() override {
				char specifier = getch();

				std::string current_command = Command::get_current_command();
				current_command += specifier;
				Command::set_current_command(current_command);

				if (exit_command_functions.count(specifier) <= 0) return;	

				exit_command_functions[specifier]();
			}

	private: void save() {
				headers.save_headers();
				endwin();
				exit(0);
		}	

	private: void force() {
				 endwin();
				 exit(0);
			 }
};

class EditModeCommand : public Command {
	public:  void initialize_command() override {
			if (edit_mode) {
				edit_mode = false;
			} else {
				edit_mode = true;
			}

			std::string message = "Edit mode is ";
			if (edit_mode) message += "on";
			else message += "off";
			
			Command::set_current_command(message);

			// When edit mode is set, tasks
			// are to be edited freerly
			
			move(0, 2);
		}
};

class AddNewHeaderCommand : public Command {
	private: std::unordered_map<char, std::function<void()>>
			 add_new_header_functions;

	public: void read_config() override {
				add_new_header_functions[AddNewHeaderBind::above] = 
					std::bind(&AddNewHeaderCommand::above, this);

				add_new_header_functions[AddNewHeaderBind::below] =
					std::bind(&AddNewHeaderCommand::below, this);

				add_new_header_functions[AddNewHeaderBind::here] = 
					std::bind(&AddNewHeaderCommand::here, this);
			}
	
	public: void initialize_command() override {
				char specifier = getch();
	
				if (add_new_header_functions.count(specifier) <= 0) return;

				add_new_header_functions[specifier]();
			}	
			
	private: void here() {
				if (headers.get_headers_flat().empty()) return;

				int x, y;
				getyx(stdscr, y, x);
				
				Header current_header = headers.get_header_flat(y);

				std::string new_header_name = get_header_name();
				if (new_header_name.empty()) {
					move(y, x); // No clue why this move is necessary, but 
								// without it, the code throws seg fault
					return;
				}
			
				HeaderBuilder new_header_builder;
				Header new_header = new_header_builder
					.path_to_header(create_path_to_self(new_header_name, current_header.get_path_to_parent()))
					.header_name(new_header_name)
					.completion_level(0)
					.path_to_parent(current_header.get_path_to_parent())
					.paths_to_children({})
					.build();
				
				if (new_header.get_path_to_parent() != ".") {
					headers.add_child_to_header(new_header.get_path_to_parent(), new_header.get_path_to_header(), y);
				}

				headers.insert_header(new_header, y);
				headers.generate_headers_flat();

				y = headers.find_header_flat(new_header.get_path_to_header());
				headers.change_colored_flat(true, y);
				headers.change_colored_flat(false, headers.find_header_flat(current_header.get_path_to_header()));
				move(y, x);
			}

	private: void below() {
				if (headers.get_headers_flat().empty()) return;

				int x, y;
				getyx(stdscr, y, x);

				Header header_above = headers.get_header_flat(y);
				
				std::string name = get_header_name(); 
				if (name.empty()) {
					move(y, x);
					return;
				}

				std::string path_to_parent = header_above.get_path_to_header();
				std::string path_to_self = create_path_to_self(name, path_to_parent);	
				header_above.insert_path_to_child(path_to_self);

				headers.replace_header(header_above, header_above.get_path_to_header());

				HeaderBuilder new_header_builder; 
				Header new_header = new_header_builder
					.header_name(name)
					.path_to_parent(path_to_parent)
					.path_to_header(path_to_self)
					.completion_level(0)
					.paths_to_children({})
					.build();

				headers.insert_header(new_header, y);
				headers.generate_headers_flat();

				y = headers.find_header_flat(path_to_self);
				move(y, x);
				headers.change_colored_flat(true, y);
				headers.change_colored_flat(false, headers.find_header_flat(path_to_parent));
			}
			 
	public: void above() {
				if (headers.get_headers_flat().empty()) return;

				int x, y;
				getyx(stdscr, y, x);
				
				std::string new_header_name = get_header_name();
				if (new_header_name.empty()) {
					move(y, x);
					return;
				}

				Header current_header = headers.get_header_flat(y);
				
				std::string old_path = current_header.get_path_to_header();
				std::string new_header_path_to_parent = current_header.get_path_to_parent();
				std::string new_header_path_to_self = create_path_to_self(new_header_name, new_header_path_to_parent);
				
				if (current_header.get_path_to_parent() != ".") { 
					headers.update_header_path_to_child
						(current_header.get_path_to_parent(), current_header.get_path_to_header(), new_header_path_to_self);
				}

				current_header.set_path_to_parent(new_header_path_to_self);
				current_header.set_path_to_header
					(create_path_to_self(current_header.get_header_name(), new_header_path_to_self));

				headers.change_header_path(old_path, current_header.get_path_to_header());
				headers.replace_header(current_header, current_header.get_path_to_header());

				std::vector<std::string> children = {current_header.get_path_to_header()};

				HeaderBuilder new_header_builder;
				Header new_header = new_header_builder
					.header_name(new_header_name)
					.path_to_header(new_header_path_to_self)
					.path_to_parent(new_header_path_to_parent)
					.completion_level(0)
					.paths_to_children(children)
					.build();

				headers.push_header(new_header);
				headers.update_paths_of_children(current_header, new_header_path_to_self);
				// headers.log_headers();	
				headers.generate_headers_flat();

				y = headers.find_header_flat(new_header_path_to_self);
				move(y, x);
				headers.change_colored_flat(true, y);
				headers.change_colored_flat(false, y + 1);
			}	
};

class ChangeCompletionLevelCommand : public Command {
	private: void down() {
			int y, x;
			getyx(stdscr, y, x);
	
			std::string path = headers.get_header_flat(y).get_path_to_header();

			headers.change_completion_level(path, y, -1);
	}

	private: void up() {
			int y, x;
			getyx(stdscr, y, x);
			
			std::string path = headers.get_header_flat(y).get_path_to_header();
	
			headers.change_completion_level(path, y, 1);
	}

	private: std::unordered_map<char, std::function<void()>> 
			 completion_level_functions;

	public: void read_config() override {
				completion_level_functions[ChangeCompletionLevelBind::up] = 
					std::bind(&ChangeCompletionLevelCommand::up, this);

				completion_level_functions[ChangeCompletionLevelBind::down] = 
					std::bind(&ChangeCompletionLevelCommand::down, this);
			}

	public: void initialize_command() override {
			char specifier = getch();
			
			if (completion_level_functions.count(specifier) <= 0) return;

			completion_level_functions[specifier]();
		}	
};

class DeleteHeaderCommand : public Command {
	private: int make_y_in_bounds(const int &y) {
				 int headers_size = headers.get_headers_flat().size();

				 if (y > 0 and y < headers_size) return y;

				 if (headers_size - y < y + 1) {
					 return headers_size - 1;
				 }

				 return 0;
			 }

	public: void initialize_command() override {
				if (headers.get_headers_flat().empty()) return;

				int y, x;
				getyx(stdscr, y, x);

				Header header = headers.get_header_flat(y);
				
				headers.delete_header(header);
				headers.generate_headers_flat();
				// headers.log_headers_flat();
				
				if (headers.get_headers_flat().empty()) return;

				y = make_y_in_bounds(y);
				headers.change_colored_flat(true, y);
				move(y, x);
			}
};

