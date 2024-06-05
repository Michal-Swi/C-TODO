#include <string>
#include <fstream>
#include <vector>

// This header exists, because I hate (don't understand the behaviour of) getline.
namespace custom_string_actions {
	std::string file_to_string(const std::string &path_to_file) {
		std::string string_file;		

		std::fstream file;
		try {
			file.open(path_to_file, std::ios::in);
		} catch (...) {
			return "";
		}
			
		char ch;

		while (file.get(ch)) {
			string_file += ch;
		}

		return string_file;
	}

	// Slow. Avoid using. 
	std::string line_to_string(const std::string &path_to_file, const int &line) {
		std::string line_from_file;		

		std::ifstream file(path_to_file);
		char ch;
		int current_line = 1;

		while (!file.eof()) {
			if (current_line == line) {
				while (file.get(ch)) {
					line_from_file += ch;
					if (ch == '\n') return line_from_file;
				}
			}

			file.get(ch);
			if (ch == '\n') current_line++;
		}

		return ""; 
	}

	std::string delete_new_line_from_string(const std::string &s) {
		if (s.empty() or s == "") return "";

		std::string s_without_new_line;

		for (const auto &ch : s) if (ch != '\n') s_without_new_line += ch;

		return s_without_new_line;
	}

	std::vector<std::string> divide_string_by_spaces(const std::string &string_to_divide) {
		if (string_to_divide.empty()) return {};

		std::vector<std::string> divided_by_spaces;
		
		std::string current_string;
		for (const auto &ch : string_to_divide) {
			if (ch == ' ' or ch == '\n') {
				divided_by_spaces.push_back(delete_new_line_from_string(current_string));
				current_string = "";
			} else if (ch != ' ' and ch != '\n') {
				current_string += ch;
			}
		} 
		
		if (!string_to_divide.empty()) divided_by_spaces.push_back(delete_new_line_from_string(current_string));

		return divided_by_spaces;
	}

	std::string get_line_from_string_file(const std::string &string_file, const int &line) {
		std::string line_from_file_string;

		int current_line = 1;
		
		int i = 0;
		for (; i < string_file.length(); i++) {
			if (current_line == line) {
				for (; i < string_file.length() and string_file[i] != '\n'; i++) {
					line_from_file_string += string_file[i];
				}

				return line_from_file_string;
			}

			if (string_file[i] == '\n') current_line++;
		}

		return "";
	}


}
