#include <fstream>
#include <vector>

std::vector<std::string> getTasks() {
	std::ifstream data("/../data/savedData.txt");
	
	std::vector<std::string> tasks;
	while (!data.eof()) {
		std::string temp;
		getline(data, temp);
		if (temp.length() == 0) continue;

		tasks.push_back(temp);
	}

	return tasks;
}

