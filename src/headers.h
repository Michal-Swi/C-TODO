#include "file_to_string.h"
#include <map>
#include <vector>

class Header {
	private:
		std::string header_name;
		int completion_level;
		std::string path_to_parent;
		std::vector<std::string> paths_to_children;
		std::string path_to_header;
		bool colored = false;
	
	public: void set_colored(const bool &colored) {
				this->colored = colored;
			}
	
	public: bool get_colored() {
				return colored;
			}
	
	public: void set_path_to_header(const std::string &path_to_header) {
				this->path_to_header = path_to_header;
			}

	public: std::string get_path_to_header() {
				return path_to_header;
			}

	public: void set_header_name(const std::string &header_name) {
				this->header_name = header_name;
			}

	public: std::string get_header_name() {
				return header_name;
			}
	
	public: void set_completion_level(const int &completion_level) {
				this->completion_level = completion_level;
			}
	
	public: int get_completion_level() {
				return completion_level;
			}
	
	public: void insert_path_to_child(const std::string &path_to_child) {
				paths_to_children.push_back(path_to_child);
			}

	public: void insert_paths_to_children(const std::vector<std::string> &paths_to_children) {
				this->paths_to_children = paths_to_children;
			}

	public: std::vector<std::string> get_paths_to_children() {
				return paths_to_children;
			}

	public: void set_path_to_parent(const std::string &path_to_parent) {
				this->path_to_parent = path_to_parent;
			}

	public: std::string get_path_to_parent() {
				return path_to_parent;
			}
};

class HeaderBuilder {
	private: Header header;
	
	public: HeaderBuilder &path_to_header(const std::string &path_to_header) {
				header.set_path_to_header(path_to_header);
				return *this;
			}

	public: HeaderBuilder &header_name(const std::string &header_name) {
				header.set_header_name(header_name);
				return *this;
			}

	public: HeaderBuilder &completion_level(const int &completion_level) {
				header.set_completion_level(completion_level);
				return *this;
			}

	public: HeaderBuilder &path_to_parent(const std::string &path_to_parent) {
				header.set_path_to_parent(path_to_parent);
				return *this;
			}

	public: HeaderBuilder &paths_to_children(const std::vector<std::string> &paths_to_children) {
				header.insert_paths_to_children(paths_to_children);
				return *this;
			}
	
	public: HeaderBuilder &colored(const bool &colored) {
				header.set_colored(colored);
				return *this;
			}

	public: Header build() {
		return header;
	}
};

struct HeaderFlat {
	int depth = 0;
	Header header;
};

class Headers {
	private: std::map<std::string, Header> headers;
	private: static std::vector<HeaderFlat> headers_flat;
	
	public: static std::vector<HeaderFlat> get_headers_flat() {
				return headers_flat;
			}

	public: static void set_headers_flat(const std::vector<HeaderFlat> &new_headers_flat) {
				headers_flat = new_headers_flat;
			}
	
	public: static Header get_header_flat(const int &y) {
				return headers_flat[y].header;
			}
	
	public: static int get_depth_flat(const int &y) {
				return headers_flat[y].depth;
			}
	
	public: static void change_header_flat(const std::string new_header_flat_name, const int &y) {
				headers_flat[y].header.set_header_name(new_header_flat_name);
			}

	private: Header read_header(std::string &path_to_header) {
					std::ifstream parent_header_file("../data/" + path_to_header);	

					std::string path, name, path_to_parent; 
					int completion_level;
					std::vector<std::string> paths_to_children;
					
					// TODO - fix the getline naming issue
					parent_header_file >> path >> name;

					parent_header_file
						>> completion_level
						>> path_to_parent;
	
					std::string path_to_child;
					while (parent_header_file >> path_to_child) {
						if (path_to_child.empty() or path_to_child == " ") {
							continue; // Skipping empty lines
						}

						paths_to_children.push_back(path_to_child);
					}
					
					HeaderBuilder header_builder;
					Header header = header_builder
						.path_to_header(path)
						.header_name(name)
						.completion_level(completion_level)
						.path_to_parent(path_to_parent)
						.paths_to_children(paths_to_children)
						.build();
	
					return header;
				}
	
	private: void read_children_of_header(std::vector<std::string> &paths_to_children,
					 std::map<std::string, Header> &new_headers, int depth = 1) {

				 for (auto &path_to_child : paths_to_children) {
					Header new_child = read_header(path_to_child);
					new_headers[path_to_child] = new_child;
					
					HeaderFlat new_header_to_render;
					new_header_to_render.header = new_child;
					new_header_to_render.depth = depth;
					headers_flat.push_back(new_header_to_render);

					std::vector<std::string> new_paths_to_children = new_child.get_paths_to_children();
					read_children_of_header(new_paths_to_children, new_headers, depth + 1);
				 }
			 }

	private: std::map<std::string, Header> read_headers() {
				std::ifstream parent_headers_file("../data/parent_headers");	
				
				std::map<std::string, Header> new_headers;

				std::string path_to_header;
				while (parent_headers_file >> path_to_header) {
					if (path_to_header.empty()) continue; // Skipping possible empty lines
					
					Header new_header = read_header(path_to_header);
					new_headers[path_to_header] = new_header;
					
					HeaderFlat new_header_to_render;
					new_header_to_render.header = new_header;
					headers_flat.push_back(new_header_to_render);

					std::vector<std::string> paths_to_children = new_header.get_paths_to_children();
					read_children_of_header(paths_to_children, new_headers);
				}

				return new_headers;
			 }

	public: Headers() {
				headers = read_headers();
			}	

	private: void save_header(Header &header) {
				std::ofstream header_out("../data" + header.get_path_to_header());

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
				std::ofstream parent_headers("../data/parent_headers");

				for (auto &[path, header]: headers) {
					if (header.get_path_to_parent() == ".") { // Is a parent header
						parent_headers << header.get_path_to_header() << '\n';		
					}

					save_header(header);	
				}
			}
	
	public: void add_new_header(const std::string &path_to_self, const std::string &name, 
					const std::string &path_to_parent, int completion_level = 0, 
					std::vector<std::string> children_paths = {}) {
				
				HeaderBuilder new_header_builder;
				Header new_header = new_header_builder
					.header_name(name)
					.path_to_header(path_to_self)
					.completion_level(completion_level)
					.path_to_parent(path_to_parent)
					.paths_to_children(children_paths)
					.build();

				headers[path_to_self] = new_header;
			}
	
	public: void change_header_name(const std::string &path_to_self, const std::string &new_name) {
				headers[path_to_self].set_header_name(new_name);
			}

	public: void change_header_name_flat(const std::string &header_name, const int &y) {
				headers_flat[y].header.set_header_name(header_name);
				headers[headers_flat[y].header.get_path_to_header()].set_header_name(header_name);
			}

	public: std::map<std::string, Header> get_headers() {
				return headers;
			}
	
	public: Header get_header(std::string &path) {
				return headers[path];
			}

	public: void insert_header(Header &header) {
				headers[header.get_path_to_header()] = header;	
			}

	public: void change_header(const Header &header, const std::string &path_to_self) {
				headers[path_to_self] = header;
			}
	
	// Note that both change_colored and change_colored_flat change colored in both flat and normal.
	public: void change_colored(const bool &colored, const std::string &path_to_header) {
				headers[path_to_header].set_colored(colored);

				for (auto &header : headers_flat) {
					if (header.header.get_path_to_header() == path_to_header) {
						header.header.set_colored(colored);
						return;
					}
				}
			}

	public: void change_colored_flat(const bool &colored, const int &y) {
				headers_flat[y].header.set_colored(colored);
				headers[headers_flat[y].header.get_path_to_header()].set_colored(colored);
			}
};

std::vector<HeaderFlat> Headers::headers_flat = {};

Headers headers;
