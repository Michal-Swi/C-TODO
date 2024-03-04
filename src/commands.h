#include <ncurses.h>
#include <iostream> // For ExitCommand.
#include <string>

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

class CreateTask : public Command {
private: std::string getTaskName() {
			std::string taskName;
			
			char keyPressed;

			// 10 is for enter, KEY_ENTER did not work for some reason.
			while (keyPressed != 10) {
				keyPressed = getch();
				if (keyPressed == 10) break;

				taskName += keyPressed;
				refresh();
			}

			return taskName;
		 }

public: void initializeCommand() override {
			const char * taskName = getTaskName().c_str();
			// Something for ptinting in the painter.h header. 
		}

