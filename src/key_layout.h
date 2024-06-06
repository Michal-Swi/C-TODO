#include "commands.h"

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

			} else {
				key_layout[command]->initialize_command();	
			}
		}

	public: KeyLayout() {
			// TODO - user defined config
			
			key_layout["qu"] = new ExitCommand();
			key_layout["a"] = new AddNewHeaderCommand();
			key_layout[":"] = new EditModeCommand();
			key_layout["ah"] = new AddNewHeaderHereCommand();
			key_layout["c"] = new ChangeCompletionLevelDownCommand();
			key_layout["v"] = new ChangeCompletionLevelUpCommand();
			key_layout["ab"] = new AddNewHeaderDownCommand();
		}	
};

KeyLayout keys;
