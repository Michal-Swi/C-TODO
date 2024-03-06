#include <ncurses.h>
#include <iostream> // For ExitCommand.
#include <string>

#define ENTER_KEY 10

class Command {
public: virtual void initializeCommand() {
		return; // Method for overriding.
	}
};

class ExitCommand : public Command {
public: void initializeCommand() override {
			endwin();
			exit(0);
		}
};

