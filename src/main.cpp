#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class Renderer {
	private: std::vector<std::string> get_tasks() {
	  	std::ifstream data("../data/savedData.txt");
	
		std::vector<std::string> tasks;
		while (!data.eof()) {
			std::string temp;
			getline(data, temp);
			if (temp.length() == 0) continue;

			tasks.push_back(temp);
		}

		return tasks;
	}

	public: void render_tasks() {
		std::vector<std::string> tasks = get_tasks();
		
		if (tasks.empty()) {
			printw("NO TASKS");
			return;
		}
		
		clear();
		for (int i = 0; i < tasks.size(); i++) {
			tasks[i] = std::to_string(i + 1) + '.' + tasks[i];
			mvprintw(i, 0, tasks[i].c_str());
			refresh();
		}

		refresh();
	}

	public: void render_test() {
		printw("RENDERING TEST");	
	}
};

void main_loop() {
	Renderer renderer;

	while (true) {
		refresh();
		renderer.render_tasks();
		char ch = getch();
	}
}

int main() {
	initscr();
	keypad(stdscr, true);
	clear();
	refresh();

	main_loop();

	endwin();

    return 0;
}

