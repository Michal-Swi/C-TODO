#include <algorithm>
#include <ncurses.h>
#include <cstdlib> // For ExitCommand.
#include <ratio>
#include <string>
#include <vector>
#include <regex>
#include <tuple>
#include "headers.h"
#include "definitions.h"

class Command {
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
	
	protected: std::string create_path_to_self(const std::string &header_name, const std::string &path_to_parent) {
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

	public: void print_line(std::string str, bool move_to_previous_position) {
				int y, x;
				getyx(stdscr, y, x);
				move(y, x - 1);
				clrtoeol();
				refresh();
				move(y, 0);
				printw(str.c_str());
				refresh();

				if (move_to_previous_position) move(y, x + 1);
			 }
	public: std::string get_command() {
			std::string command;

			char ch;

			int current_x, current_y;
			getyx(stdscr, current_y, current_x);
			
			int max_x, max_y;
			getmaxyx(stdscr, max_y, max_x);

			move(max_y - 1, 0);

			ch = getch();
			clrtoeol();
	
			if (ch == ENTER_KEY) {
				move(current_y + 1, 0);
				refresh();

				return "Invalid command!"; 
			}
			
			addch(ch);
			command += ch;

			while (ch != ENTER_KEY) {
				ch = getch();
				
				if (ch == ENTER_KEY) {
					move(current_y + 1, 0);
					return command;
				}
				
				command += ch;

				addch(ch);
				refresh();
			}
			
			return "";
		}


	protected: std::string get_header_name() {
			std::string header_name;
			
			int x, y;
			getyx(stdscr, y, x);

			int max_y, max_x;
			getmaxyx(stdscr, max_y, max_x);
			move(max_y - 1, 0);
			clrtoeol();

			char ch;
			ch = getch();
			
			if (ch == ENTER_KEY) return ""; // If the header_name is just 
											// new line we just exit
			if (int(ch) != BACKSPACE_KEY) header_name = ch;
			
			print_line(header_name, false);

			while (ch != ENTER_KEY) {
				char ch = getch();
				bool move_to_previous_position = false;
				
				switch(ch) {
					case ENTER_KEY:
						return header_name;
					case BACKSPACE_KEY: 
						delete_character(header_name);
						print_line(header_name, false);
						break;	
					case ARROW_LEFT:
						getyx(stdscr, y, x);
		
						move(y, x - 1);
						break;
					case ARROW_RIGHT:
						getyx(stdscr, y, x);
	
						if (x >= header_name.length()) continue;
	
						move(y, x + 1);
						break;
				}

				add_character_to_header(header_name, ch, move_to_previous_position);
				print_line(header_name, move_to_previous_position);
			}

			return ""; // Done just for LSP purposes
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

	private: std::vector<std::pair<std::string, std::string>> 
			 get_config_definitions() {
			std::ifstream config_data("../data/config");	

			// Given a file structured;
			// key: definition
			// this regex will mach the 
			// key and the definition
			std::regex config_key_regex("^([^:]+):(.*)$");
			
			std::vector<std::pair<std::string, std::string>> definitions;
			while (!config_data.eof()) {
				std::string config_data_line;
				config_data >> config_data_line;
				
				std::smatch matched_key;
				std::regex_match(config_data_line, matched_key, config_key_regex);
				definitions.push_back({matched_key[0], matched_key[1]});
			}
			
			return definitions;
		 }
};

bool Command::edit_mode = false;
std::string Command::current_command = "";

class ExitingCommands : public Command {
	public: void initialize_command() override {
				char ch = getch();
			}
};


class ExitCommand : public Command {
	public: void initialize_command() override {
			headers.save_headers();
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
	public: void initialize_command() override {
			std::string header_name = get_header_name();
			if (header_name == "") return;
			
			int current_y, current_x;
			getyx(stdscr, current_y, current_x);

			Header neigbouring_header = headers.get_header_flat(current_y);
			
			HeaderBuilder new_header_builder;
			Header new_header = new_header_builder
				.header_name(header_name)
				.path_to_header(create_path_to_self(header_name, neigbouring_header.get_path_to_parent()))
				.completion_level(0) // New header can't have a different completion level
				.path_to_parent(neigbouring_header.get_path_to_parent())
				.paths_to_children({}) // New headers cant't have children yet 
				.build();

			headers.insert_header(new_header, current_y);
		}
};

class AddNewHeaderHereCommand : public Command {
	public: void initialize_command() override {
				if (headers.get_headers_flat().empty()) return;

				int x, y;
				getyx(stdscr, y, x);
				
				Header current_header = headers.get_header_flat(y);

				std::string new_header_name = get_header_name();
			
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
			}
};

class AddNewHeaderDownCommand : public Command {
	public: void initialize_command() override {
				if (headers.get_headers_flat().empty()) return;

				int x, y;
				getyx(stdscr, y, x);

				Header header_above = headers.get_header_flat(y);
				
				std::string name = get_header_name(); 
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
			}
};

class ChangeCompletionLevelUpCommand : public Command {
	public: void initialize_command() override {
		int y, x;
		getyx(stdscr, y, x);
		
		std::string path = headers.get_header_flat(y).get_path_to_header();

		headers.change_completion_level(path, y, 1);
	}
};

class ChangeCompletionLevelDownCommand : public Command {
	public: void initialize_command() override {
		int y, x;
		getyx(stdscr, y, x);

		std::string path = headers.get_header_flat(y).get_path_to_header();

		headers.change_completion_level(path, y, -1);
	}
};
