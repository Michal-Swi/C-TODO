#include <map>
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

	public: Header build() {
		return header;
	}
};

class Headers {
	private: std::map<std::string, Header> headers;
	private: static std::vector<Header> headers_flat;
	
	public: static std::vector<Header> get_headers_flat() {
				return headers_flat;
			}
	
	public: static void set_headers_flat(const std::vector<Header> &headers_flat) {
				Headers::headers_flat = headers_flat;
			}
	
	public: void make_new_headers_flat() {
				std::vector<Header> new_headers_flat;

				for (auto &[path, header] : headers) {
					new_headers_flat.push_back(header);
				}

				headers_flat = new_headers_flat;
			 }
	
	private: void log_all_headers() {
				 std::fstream log;
				 log.open("log.log", std::ios::app);

				 for (auto &[path, header] : headers) {
					 log << "Header path: " << path << '\n';
					 log << "Header name: " << header.get_header_name() << '\n';
					 log << "Completion level: " << header.get_completion_level() << '\n';
					 log << '\n';
				 }
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

	private: std::map<std::string, Header> read_headers() {
				std::ifstream parent_headers_file("../data/parent_headers");	
				
				std::map<std::string, Header> new_headers;

				std::string header_path;
				while (parent_headers_file >> header_path) {
					if (header_path.empty() or header_path == " ") {
						continue; // Skipping empty lines
					}
					
					Header new_parnet_header = read_header(header_path);
					new_headers[new_parnet_header.get_path_to_header()] = new_parnet_header;
				}
				
				for (auto &[path, header] : new_headers) {
					for (auto &header_path : header.get_paths_to_children()) {
						Header new_header = read_header(header_path);
						new_headers[new_header.get_path_to_header()] = new_header;
					}
				}

				return new_headers;
			 }

	public: Headers() {
				headers = read_headers();
				log_all_headers();
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
				for (auto &[path, header]: headers) {
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

	public: std::map<std::string, Header> get_headers() {
				return headers;
			}
	
	public: Header get_header(std::string &path) {
				return headers[path];
			}
	
	public: Header get_header_flat(const int &position) {
				return headers_flat[position];
			}

	public: void insert_header(Header &header) {
				headers[header.get_path_to_header()] = header;	
			}
};

std::vector<Header> Headers::headers_flat = {};

Headers headers;
