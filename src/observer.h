#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>



class Observer {
	
public:
	virtual void print(const std::vector<std::string>&, const std::string&) = 0;
	virtual ~Observer() = default;
};


class FileObserver : public Observer {

	static int file_id;
	
	virtual void print(const std::vector<std::string>& comm, const std::string& time) {
		std::ofstream file;
		++file_id;
		std::string path("c:\\test\\bulk" + std::to_string(file_id) + time + ".log");
		file.open(path);
		for (auto& command : comm) {
			file << command << "\n";
		}
		file.close();
	}

};

class TerminalObserver : public Observer {
	virtual void print(const std::vector<std::string>& comm, const std::string&) {
		std::cout << "Bulk: ";
		bool first = true;
		for (auto& command : comm) {
			if (!first) { std::cout << ","; }
			std::cout << command;
			first = false;
		}
		std::cout << std::endl;
	}

};


int FileObserver::file_id = 0; 
