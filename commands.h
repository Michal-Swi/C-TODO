#include <map>
#include <ncurses.h>
#include <string>
#include "config.h"

class CurrentCommand {
private:
	std::string currentCommand;
public:
	void setCurrentCommand(std::string currentCommand) {
		this->currentCommand = currentCommand;
	}

	std::string getCurrentCommand() {
		return currentCommand;
	}
};

class Command {
public:
	virtual void initializeCommand() {
		// For function overriding.
		return;
	}	
};

class WriteCommand : public Command {
public:
	void initializeCommand() override {
		printw("napis");
	}
};

// Class for adding assigments to the list.
class AddAssigment : Command {
public:
	void initializeCommand() override {
		clear();
					
	}
};

std::map<char, Command*> commands;

void initializeCommands() {
	commands[Config::NewAssigment] = new WriteCommand();
}

