#include "commands.h"
#include <fstream>

class KeyLayout {
	public: std::map<std::string, Command*> key_layout; 

	public: void execute_command(std::string command) {
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

	public: KeyLayout() {
			// TODO - user defined config
			
			key_layout["a"] = new AddNewHeaderCommand();
			key_layout["d"] = new DeleteHeaderCommand();
			key_layout["c"] = new ChangeCompletionLevelCommand();
			key_layout["e"] = new ExitCommand();
		}	

	public: ~KeyLayout() {
				for (auto &[key, command] : key_layout) {
					delete command;
				}
			}

};

KeyLayout keys;
