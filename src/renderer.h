#include <ios>
#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include "edit_mode.h"

namespace CompletionLevels {
	std::unordered_map<int, std::string> completion_levels;
	
	void fill_completion_levels() {
		completion_levels[0] = "[ ]";
		completion_levels[1] = "[-]";
		completion_levels[2] = "[X]";
	}
}

namespace DoubleRenderChecker {
	std::unordered_map<std::string, bool> double_render_checker;
}

class Renderer {
	private: void render_children(std::vector<Header> &flat_headers, Header header, int depth = 0) { // Depth - amount of tabs
				std::vector<std::string> children_paths = header.get_paths_to_children();	

				for (int i = 0; i < depth; i++) printw("\t");

				for (auto &child_path : children_paths) {
					

					if (DoubleRenderChecker::double_render_checker[child_path]) continue;
					DoubleRenderChecker::double_render_checker[child_path] = true;

					printw(headers.get_header(child_path).get_header_name().c_str());
					render_children(flat_headers, headers.get_header(child_path), depth + 1) ;
				}
			 }

	public: void render_all_headers
			(std::map<std::string, Header> headers_to_render, 
			 std::string current_command) {
		
		if (headers_to_render.empty()) {
			printw("NO TASKS");
			return;
		}
		
		std::vector<Header> flat_headers;
		std::unordered_map<std::string, bool> duplicate_rendering_checker;
		for (auto &[path, header] : headers_to_render) {
			std::string output;

			output += header.get_header_name();
			output += ' ';
			output += CompletionLevels::completion_levels[header.get_completion_level()];
			
			DoubleRenderChecker::double_render_checker[path] = true;
			
			flat_headers.push_back(header);

			render_children(flat_headers, header);
		}

		DoubleRenderChecker::double_render_checker.clear();

		int x, y;
		getyx(stdscr, y, x);

		move(getmaxy(stdscr), getmaxx(stdscr));

		printw(current_command.c_str());
		move(y, x);
	}	
	
	private: std::string get_text() {
		std::string text;
		
		char ch;
		while (ch != ENTER_KEY) {
			ch = getch();
			if (ch == ENTER_KEY and text.empty()) ch = 'p'; // Something different 
															// from ENTER_KEY so that
															// the loop continues
			else if (ch != ENTER_KEY) {
				text += ch;
				addch(ch);
			}
		}

		int y, x;
		getyx(stdscr, y, x);
		
		move(y, 0); // Moves to the start of the line to clear it
		clrtoeol(); // Clears the line from addch();
		refresh();

		return text;
	}

	// Outputs text written by user
	public: void render_user_text() {
		std::string user_text = get_text();
		
		std::fstream headers;
		headers.open("../data/headers.txt", std::ios_base::app);

		printw(user_text.c_str());
	}
};

