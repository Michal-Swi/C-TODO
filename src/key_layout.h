#include "commands.h"
#include <cstdlib>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "toml.hpp"

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
	
	private: std::string get_home_path() {
				 std::string home_path;

				 try {
					 home_path = std::getenv("HOME");
				 } catch (...) {
					 throw std::runtime_error("HOME enviroment variable not set.");
				 }
				
				 return home_path;
			 }

	private: void read_config() {
				toml::table config;
				
				std::string path = get_home_path() + "/.config/C-TODO/config.toml";

				try {
					config = toml::parse_file(path);
				} catch (const toml::parse_error &err) {
					// No config to read or some error, either way we can just exit

					std::fstream log("log.log", std::ios::app);
					log << err.what() << std::endl;
					log.close();

					return;
				}

				// I believe hard coding it is the way, I just don't see how to do this
				// in a different way.

				std::string add_new_header_key = 
					config["AddNewHeaderCommand"].value_or("a");

				std::string delete_header_key = 
					config["DeleteHeaderCommand"].value_or("d");

				std::string change_completion_key = 
					config["ChangeCompletionLevelCommand"].value_or("c");

				std::string exit_key = 
					config["ExitCommand"].value_or("e");

				key_layout[add_new_header_key] = new AddNewHeaderCommand();
				key_layout[delete_header_key] = new DeleteHeaderCommand();
				key_layout[change_completion_key] = new ChangeCompletionLevelCommand();
				key_layout[exit_key] = new ExitCommand();
			 }

	public: KeyLayout() {
			key_layout["a"] = new AddNewHeaderCommand();
			key_layout["d"] = new DeleteHeaderCommand();
			key_layout["c"] = new ChangeCompletionLevelCommand();
			key_layout["e"] = new ExitCommand();

			read_config();
		}	
	
	public: ~KeyLayout() {
				for (auto &[key, command] : key_layout) {
					delete command;
				}
			}
};

KeyLayout keys;
