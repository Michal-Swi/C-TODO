#include "commands.h"

class KeyLayout {
	public: ~KeyLayout() {}

	public: std::map<std::string, Command*> key_layout; 

	public: void execute_command(std::string command) {
			std::fstream file;
			file.open("log.log", std::ios::app);

			file << command << '\n';

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

	// Constructor assigning the classes to keys
	public: KeyLayout() {
			// Assigning is temporary for development purposes only! Change later! 
			
			key_layout["qu"] = new ExitCommand();
			key_layout["a"] = new AddNewHeaderCommand();
			key_layout[":"] = new EditModeCommand();
		}	
};
