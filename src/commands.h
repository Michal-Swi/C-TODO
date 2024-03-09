#include <ncurses.h>
#include <cstdlib> // For ExitCommand.
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <regex>

#define ENTER_KEY 10

class Command {
public: virtual void initialize_command() {
			return; // Method for overriding.
		}

public: void map_key_layout() {
			std::ifstream config_data("../data/config");	
			
			std::vector<std::string> config_data_vector;
			std::regex config_key_regex("^([^:]+)");

			while (!config_data.eof()) {
				std::string config_data_line;
				config_data >> config_data_line;

			}
			
			
		}

public: std::map<std::string, Command*> key_layout; 

};

class ExitCommand : public Command {
public: void initialize_command() override {
			endwin();
			exit(0);
		}
};

class EditModeCommand : public Command {
public: virtual void initialize_command() override {
			return;	
		}
};
