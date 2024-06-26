#include "commands.h"
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class KeyLayout {
	public: std::map<std::string, Command*> key_layout; 

	public: void execute_command(const std::string &command) {
			if (!(key_layout.count(command) > 0)) {
				int current_y, current_x;
				getyx(stdscr, current_y, current_x);

				int max_y;
				max_y = getmaxy(stdscr);

				move(max_y - 1, 0);

				Command::set_current_command("Invalid command!");

				move(current_y, current_x);
				return;
			}

			key_layout[command]->initialize_command();	
		}

	private: std::vector<std::pair<std::string, std::string>>
			 get_config_definitions() {
			std::ifstream config_data("~/.config/C-TODO/config");	

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

	private: void override_key_layout(const std::vector<std::pair<std::string, std::string>> &config,
				std::map<std::string, Command*> &key_layout) {
					
				for (auto &[definition, key] : config) {

				}
			 }

	public: KeyLayout() {
			key_layout["a"] = new AddNewHeaderCommand();
			key_layout["d"] = new DeleteHeaderCommand();
			key_layout["c"] = new ChangeCompletionLevelCommand();
			key_layout["e"] = new ExitCommand();

			// std::vector<std::pair<std::string, std::string>> config = get_config_definitions();
		}	
	
	public: ~KeyLayout() {
				for (auto &[key, command] : key_layout) {
					delete command;
				}
			}

};

KeyLayout keys;
