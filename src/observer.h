#pragma once
#include <fstream>
#include <iostream>
#include <vector>


class Observer {
public:
	virtual void print(const std::vector<std::string>&, const std::string&) = 0;
	virtual ~Observer() = default;
};


class FileObserver : public Observer {
	virtual void print(const std::vector<std::string>& comm, const std::string& time) {
		std::ofstream file;
		std::string path("c:\\test\\bulk" + time + ".log");
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
