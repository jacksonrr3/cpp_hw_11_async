#pragma once
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <ctime>
#include <sstream>
#include "observer.h"


using obs_vec_ptr = std::shared_ptr<std::vector<std::unique_ptr<Observer>>>;

class Command {
	obs_vec_ptr _obs;
	std::vector<std::string> _comm;

	std::size_t _block_size;
	std::size_t _comm_counter = 0;
	std::size_t _bracket_counter = 0;
	bool _is_reg = true;
	std::string _time;

public:
	Command(std::size_t N, obs_vec_ptr obs):_block_size(N), _obs(obs){}

	~Command() {
		if (_is_reg) {
			if (_comm_counter) { 
				notify(); 
			}
		}
	}
	
	void subscribe(std::unique_ptr<Observer>&& obs) {
		_obs->emplace_back(std::move(obs));
	}

	void setTime() {
		std::time_t temp_time;
		temp_time = std::time(0);
		std::stringstream ss;
		ss << temp_time;
		ss >> _time;
	}

	void set_mode(bool b) {
		_is_reg = b;
		_bracket_counter = 1;
	}

	void notify() {
		for (auto& u : *_obs) {
			u->print(_comm, _time);
		}
		_comm.clear();
		_time.clear();
	}

	void add_command(const std::string& s, bool& u) {

			if (s[0] == '{') {
				if (_comm_counter) {
					notify();
					_comm_counter = 0;
					//_is_reg = false;
					u = false;
				}

				++_bracket_counter;
				//_is_reg = false;
				return;
			}
			else if (s[0] == '}') {
				--_bracket_counter;
				if (!_bracket_counter) {
					notify();
					//_is_reg = true;  
					u = true;
				}
			}

			else {
				if (_comm.empty()) { setTime(); }
				_comm.push_back(s);
				if (_is_reg) {
					++_comm_counter;
					if (_comm_counter == _block_size) {
						notify();
						_comm_counter = 0;
					}
				}
			}
	}



};


