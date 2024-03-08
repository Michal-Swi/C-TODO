#include <ncurses.h>
#include <iostream> // For ExitCommand.
#include <string>
#include <map>

#define ENTER_KEY 10

class Command {
public: virtual void initialize_command() {
		return; // Method for overriding.
	}
	
public: std::map<std::string, Command*> key_layout; 
};

class ExitCommand : public Command {
public: void initialize_command() override {
			endwin();
			exit(0);
		}
};

