#include <ncurses.h>
#include <cstdlib> // For ExitCommand.
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <regex>
#include <tuple>

#define ENTER_KEY 10

class Command {
	public: std::string get_command() {
			std::string command;

			char ch;

			int current_x, current_y;
			getyx(stdscr, current_y, current_x);
			
			int max_x, max_y;
			getmaxyx(stdscr, max_y, max_x);

			move(max_y - 1, 0);
			clrtoeol();

			ch = getch();
			if (ch == ENTER_KEY) {
				printw("Invalid command!");
				move(current_y, current_x);
				refresh();

				return "";
			}

			while (ch != ENTER_KEY) {
				ch = getch();
				
				if (ch == ENTER_KEY) {
					move(current_y, current_x);
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

private: std::vector<std::pair<std::string, std::string>> get_config_definitions() {
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

bool Command::edit_mode = true;
std::string Command::current_command = "";

class ExitCommand : public Command {
public: void initialize_command() override {
			endwin();
			exit(0);
		}
};

class EditModeCommand : public Command {
public:  void initialize_command() override {
			edit_mode = true;	
			// When edit mode is set, tasks
			// are to be edited freerly
		}
};

class AddNewHeaderCommand : public Command {
private: std::string get_header_name() {
			std::string header_name;

			char ch;
			ch = getch();
			
			if (ch == ENTER_KEY) return ""; // If the header_name is just 
											// new line we just exit
			
			header_name = ch;

			int max_y, max_x;
			getmaxyx(stdscr, max_y, max_x);
			move(max_y - 1, 0);

			while (ch != ENTER_KEY) {
				char ch = getch();

				if (ch == ENTER_KEY) return header_name;
				header_name += ch;
			}

			return ""; // Done just for LSP purposes
		 }

public: void initialize_command() override {
			std::string header_name = get_header_name();

			if (header_name == "") return;

			std::fstream headers;
			headers.open("../data/headers.txt", std::ios_base::app);
			headers << header_name;
			headers.close();
		}
};

class KeyLayout {
public: ~KeyLayout() {}

public: static std::map<std::string, Command> key_layout; 

public: void execute_command(std::string command) {
			key_layout[command].initialize_command();	
		}

public: void map_key_layout() {
			// Temporary for development purposes only! Change later! 
			
			ExitCommand exit_temp;
			EditModeCommand edit_mode_temp;
			AddNewHeaderCommand add_new_header_temp;

			key_layout["q"] = exit_temp;
			key_layout[":"] = edit_mode_temp;
			key_layout["a"] = add_new_header_temp;
		}	
};

std::map<std::string, Command> KeyLayout::key_layout;
