#include <string>
#include <fstream>

// This header exists, because I hate (don't understand the behaviour of) getline.
namespace std {
	std::string file_to_string(const std::string &file_name) {
		std::string string_file;		

		std::ifstream file(file_name);
		char ch;

		while (file.get(ch)) {
			string_file += ch;
		}

		return string_file;
	}

	// Slow. Avoid using. 
	std::string line_to_string(const std::string &file_name, const int &line) {
		std::string line_from_file;		

		std::ifstream file(file_name);
		char ch;
		int current_line = 1;

		while (!file.eof()) {
			if (line == current_line) {
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
}
