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
public: virtual void initialize_command() {
			return; // Method for overriding.
		}

public: std::map<std::string, Command*> key_layout; 

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

private: Command* match_command_with_key(std::string key) {
			
		 }

public: void map_key_layout() {
			std::vector<std::pair<std::string, std::string>> 
				definitions = get_config_definitions(); 	
				
			for (const auto& definition : definitions) {
			}
		}
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
