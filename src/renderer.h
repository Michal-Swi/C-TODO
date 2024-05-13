#include <cassert>
#include <cstddef>
#include <ios>
#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include "edit_mode.h"

class CompletionLevels {
	private: static std::unordered_map<int, std::string> completion_levels;
	
	public: static void fill_completion_levels() {
		completion_levels[0] = "[ ]";
		completion_levels[1] = "[-]";
		completion_levels[2] = "[X]";
	}

	public: static std::string get_completion_level(int completion_level) {
		assert(completion_level > 0 or completion_level < 4);
		return completion_levels[completion_level];
	}
};

std::unordered_map<int, std::string> CompletionLevels::completion_levels;

class Renderer {
	private: std::string get_full_task_output(Header header) {
				std::string output;
				
				output += header.get_header_name();
				output += " - ";
				output += CompletionLevels::get_completion_level(header.get_completion_level());

				return output;
			 }

	public: void render_headers(std::vector<HeaderFlat> headers_to_render, 
					const std::string &current_command) {
		
		if (headers_to_render.empty()) {
			printw("NO TASKS");
			return;
		}

		int x = 0, y = 0;
		move(y, x);

		for (auto &header_to_render : headers_to_render) {
			if (header_to_render.header.get_colored()) {
				attron(COLOR_PAIR(2));	
			}

			for (int i = 0; i < header_to_render.depth; i++) printw("\t");
			std::string to_render = get_full_task_output(header_to_render.header);
			printw(to_render.c_str());
			refresh();

			attron(COLOR_PAIR(1));
			
			y++;
			move(y, x);
		}

		move(getmaxy(stdscr) - 1, 0);	
		printw(current_command.c_str());
		refresh();
		move(0, 0);
	}	
};
