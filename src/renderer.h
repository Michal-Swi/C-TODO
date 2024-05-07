#include <cassert>
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

namespace DoubleRenderChecker {
	std::map<std::string, bool> double_render_checker; // We don't want headers to render twice
}

class Renderer {
	private: std::string get_full_task_output(Header header) {
				std::string output;
				
				output += header.get_header_name();
				output += " - ";
				output += CompletionLevels::get_completion_level(header.get_completion_level());

				return output;
			 }

	private: void render_children_of_header(int &y, Header header, int depth = 1) { // Header not a reference (?)
				y++;
				move(y, 0);

				for (int i = 0; i < depth; i++) printw("\t");
		
				for (auto &path_to_child : header.get_paths_to_children()) {
					if (DoubleRenderChecker::double_render_checker[path_to_child]) continue;
					DoubleRenderChecker::double_render_checker[path_to_child] = true;
					
					std::string output = get_full_task_output(headers.get_header(path_to_child));
					printw(output.c_str());

					render_children_of_header(y, headers.get_header(path_to_child), depth + 1);
				}
			 }

	public: void render_all_headers(std::map<std::string, Header> all_headers, 
					const std::string &current_command) {
		
		if (all_headers.empty()) {
			printw("NO TASKS");
			return;
		}

		int x, y;
		getyx(stdscr, y, x);
			
		for (auto &[path, header] : all_headers) {
			if (DoubleRenderChecker::double_render_checker[path]) continue;
			DoubleRenderChecker::double_render_checker[path] = true;
			
			move(y, 0);

			std::string output = get_full_task_output(header);
			printw(output.c_str());

			render_children_of_header(y, header);
		}

		printw(current_command.c_str());
		move(y, x);
		
		DoubleRenderChecker::double_render_checker.clear();
	}	
};
