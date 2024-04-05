#include <algorithm>
#include <ncurses.h>
#include <cstdlib> // For ExitCommand.
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <regex>
#include <tuple>
#include <vector>
#include "definitions.h"

class Command {
	private: static std::vector<std::string> headers;

	public: static void read_headers() {
	  	std::ifstream data("../data/headers.txt");
	
		while (!data.eof()) {
			std::string temp;
			getline(data, temp);
			if (temp.length() == 0) continue;

			headers.push_back(temp);
		}
	}

	public: static std::string get_header(int i) {
				try {
					return headers[i];
				} catch(...) {
					return "";
				}
			}

	public: static void add_to_headers(std::string header) {
				headers.push_back(header);
			}

	public: static std::vector<std::string> get_headers() {
				return headers;
			}

	public: static void change_header(int &i, std::string &header) {
				headers[i] = header;
			}

	public: static std::string get_last_header() {
				return headers[headers.size() - 1];
			}

	public: static int get_headers_size() {
				return headers.size();
			}

	public: static void save_headers() {
				std::ofstream out("../data/headers.txt");

				for (int i = 0; i < headers.size(); i++) {
					out << headers[i] << "\n";
				}
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
				printw("Invalid command!");
				move(current_y + 1, 0);
				refresh();

				return "";
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

	public: ~Command() {}

	public: static bool edit_mode;

	public: static std::string current_command;

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
std::vector<std::string> Command::headers;

class ExitCommand : public Command {
	public: void initialize_command() override {
			save_headers();
			endwin();
			exit(0);
		}
};

class EditModeCommand : public Command {
	public:  void initialize_command() override {
			edit_mode ? edit_mode = false : edit_mode = true;	
			edit_mode ? printw("Edit mode is on") : printw("Edit mode is off");
			// When edit mode is set, tasks
			// are to be edited freerly
			
			move(0, 2);
		}
};

class AddNewHeaderCommand : public Command {
	private: std::string get_header_name() {
			std::string header_name;
			
			char ch;
			ch = getch();
			
			if (ch == ENTER_KEY) return ""; // If the header_name is just 
											// new line we just exit
			if (int(ch) != BACKSPACE_KEY) header_name = ch;
			
			print_line(header_name, false);

			while (ch != ENTER_KEY) {
				char ch = getch();
				bool move_to_previous_position = false;

				if (ch == ENTER_KEY) return header_name;
				else if (ch == BACKSPACE_KEY) {
					delete_character(header_name);
					print_line(header_name, false);
					continue;	
				} else if (ch == ARROW_LEFT) {
					int x, y;
					getyx(stdscr, y, x);

					move(y, x - 1);
					continue;
				} else if (ch == ARROW_RIGHT) {
					int x, y;
					getyx(stdscr, y, x);

					if (x >= header_name.length()) continue;

					move(y, x + 1);
					continue;
				}

				add_character_to_header(header_name, ch, move_to_previous_position);
				print_line(header_name, move_to_previous_position);
			}

			return ""; // Done just for LSP purposes
		 }

	public: void initialize_command() override {
			std::string header_name = get_header_name();
			
			if (header_name == "") return;
			
			add_to_headers(header_name);
		}
};
