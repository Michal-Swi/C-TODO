#include <string>
#include <vector>
#include <fstream>

class Header {
	private:
		std::string header_name;
		int completion_level;
		
		std::vector<std::string> children; // Holds the names of the headers "below" 
		
		std::string parent; // Hold the name of the header above, empty if it is 
						 // the most above one
						
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
	
	public: void insert_child(std::string child) {
				children.push_back(child);
			}

	public: void insert_children(std::vector<std::string> children) {
				this->children = children;
			}

	public: std::vector<std::string> get_children() {
				return children;
			}

	public: void set_parent(std::string parent) {
				this->parent = parent;
			}

	public: std::string get_parent() {
				return parent;
			}
};

class HeaderBuilder {
	private: Header header;
	
	public: HeaderBuilder &header_name(std::string header_name) {
				header.set_header_name(header_name);
				return *this;
			}

	public: HeaderBuilder &completion_level(int completion_level) {
				header.set_completion_level(completion_level);
				return *this;
			}

	public: HeaderBuilder &parent(std::string parent) {
				header.set_parent(parent);
				return *this;
			}

	public: HeaderBuilder &children(std::vector<std::string> children) {
				header.insert_children(children);
				return *this;
			}

	public: Header build() {
		return header;
	}
};

class Headers {
	private: std::vector<Header> headers;
	private: std::vector<std::string> parent_headers; // The most "above" headers
	
	public: void read_father_headers() {
				std::ifstream data("../data/headers.txt");

				while (!data.eof()) {
					std::string parent_header;
					getline(data, parent_header);

					parent_headers.push_back(parent_header);
				}
			}
	
	private: std::vector<std::string> get_all_header_names() {
				std::ifstream headers("../data/all_headers.txt");
				std::vector<std::string> all_headers;	

				while (!headers.eof()) {
					std::string header;
					headers >> header;

					all_headers.push_back(header);	
				}
				
				return all_headers;
			}	

	private: Header read_header(std::string header_name) {
				HeaderBuilder header_builder;
			
				std::ifstream header_data("../data/" + header_name);	

				std::string parent;
				int completion_level;
				std::vector<std::string> children;

				getline(header_data, parent); 
				header_data >> completion_level;

				while (!header_data.eof()) {
					std::string child;
					getline(header_data, child);

					children.push_back(child);
				}
				
				Header header = header_builder
					.header_name(header_name)
					.completion_level(completion_level)
					.parent(parent)
					.children(children)
					.build();

				return header;
			}

	public: void fill_headers() {
				std::vector<std::string> all_header_names 
					= get_all_header_names();		
			
				for (const auto &header_name : all_header_names) {
					Header temp = read_header(header_name);						
					headers.push_back(temp);
				}

			}

	public: void save_headers() {

			}
};
