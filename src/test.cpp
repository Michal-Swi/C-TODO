#include "custom_string_actions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
	std::string path = "../data/parent_headers";
	std::string all_files = custom_string_actions::file_to_string(path);	

	std::vector<std::string> all_children;
	
	for (int i = 1; i <= 3; i++) {
		std::string path =
			custom_string_actions::
			delete_new_line_from_string(custom_string_actions::get_line_from_string_file(all_files, i));

		path = "../data/" + path;

		std::string file_content = custom_string_actions::file_to_string(path);

		std::string children_line =
			custom_string_actions::
			delete_new_line_from_string(custom_string_actions::
					get_line_from_string_file(file_content, 5));

		std::vector<std::string> vc = custom_string_actions::divide_string_by_spaces(children_line);

		all_children.insert(all_children.end(), vc.begin(), vc.end());
	}
	
	for (const auto &s : all_children) {
		std::cout << s << std::endl;
	}

	return 0;
}
