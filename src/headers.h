#include <string>
#include <vector>
#include <fstream>

class Header {
	private:
		std::string header_name;
		int completion_level;
		std::string path_to_parent;
		std::vector<std::string> paths_to_children;
		std::string path_to_header;
	
	public: void set_path_to_header(std::string path_to_header) {
				this->path_to_header = path_to_header;
			}

	public: std::string get_path_to_header() {
				return path_to_header;
			}

	public: void set_header_name(std::string header_name) {
				this->header_name = header_name;
			}

	public: std::string get_header_name() {
				return header_name;
			}
	
	public: void set_completion_level(int completion_level) {
				this->completion_level = completion_level;
			}
	
	public: int get_completion_level() {
				return completion_level;
			}
	
	public: void insert_path_to_child(std::string path_to_child) {
				paths_to_children.push_back(path_to_child);
			}

	public: void insert_paths_to_children(std::vector<std::string> paths_to_children) {
				this->paths_to_children = paths_to_children;
			}

	public: std::vector<std::string> get_paths_to_children() {
				return paths_to_children;
			}

	public: void set_path_to_parent(std::string path_to_parent) {
				this->path_to_parent = path_to_parent;
			}

	public: std::string get_path_to_parent() {
				return path_to_parent;
			}
};

class HeaderBuilder {
	private: Header header;
	
	public: HeaderBuilder &path_to_header(std::string path_to_header) {
				header.set_path_to_header(path_to_header);
				return *this;
			}

	public: HeaderBuilder &header_name(std::string header_name) {
				header.set_header_name(header_name);
				return *this;
			}

	public: HeaderBuilder &completion_level(int completion_level) {
				header.set_completion_level(completion_level);
				return *this;
			}

	public: HeaderBuilder &path_to_parent(std::string path_to_parent) {
				header.set_path_to_parent(path_to_parent);
				return *this;
			}

	public: HeaderBuilder &paths_to_children(std::vector<std::string> paths_to_children) {
				header.insert_paths_to_children(paths_to_children);
				return *this;
			}

	public: Header build() {
		return header;
	}
};

class Headers {
	private: std::vector<Header> headers;
	private: std::vector<std::string> parent_headers; 
	
	private: Header read_header(std::string &path_to_header) {
					std::ifstream parent_header_file(path_to_header);	
					
					std::string path, header_name, path_to_parent; 
					int completion_level;
					std::vector<std::string> paths_to_children;

					parent_header_file >> path;
					getline(parent_header_file, header_name);
					parent_header_file
						>> completion_level
						>> path_to_parent;

					while (!parent_header_file.eof()) {
						std::string path_to_child;
						parent_header_file >> path_to_child;

						paths_to_children.push_back(path_to_child);
					}

					HeaderBuilder header_builder;
					Header header = header_builder
						.path_to_header(path)
						.header_name(header_name)
						.completion_level(completion_level)
						.path_to_parent(path_to_parent)
						.paths_to_children(paths_to_children)
						.build();

					return header;
				}

	private: void read_headers() {
				for (auto &header : headers) {
					for (auto &path_to_child : header.get_paths_to_children()) {
						Header header = read_header(path_to_child);
						headers.push_back(header);
					}
				}
			 }

	// Gets all the most above headers 
	public: Headers() {
				std::ifstream parent_headers_file("../data/headers");
				
				while (!parent_headers_file.eof()) {
					std::string path_to_header;
					parent_headers_file >> path_to_header;
					
					Header parent_header = read_header(path_to_header);	
					headers.push_back(parent_header);
				}

				read_headers(); // Reads the rest of the headers
			}	
	
	private: void save_header(Header &header) {
				std::ofstream header_out(header.get_path_to_header());

				header_out
					<< header.get_path_to_header() << '\n'
					<< header.get_header_name() << '\n' 
					<< header.get_completion_level() << '\n' 
					<< header.get_path_to_parent() << '\n';

				for (const auto &children_paths : header.get_paths_to_children()) {
					header_out << children_paths << ' ';
				}
			 }

	public: void save_headers() {
				for (auto &header : headers) {
					save_header(header);	
				}
			}
};
