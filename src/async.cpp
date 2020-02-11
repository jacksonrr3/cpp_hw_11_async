#include <map>
#include <algorithm>
#include <functional>
#include "async.h"
#include "command.h"

namespace async {
	

	obs_vec_ptr observers = std::make_shared<std::vector<std::unique_ptr<Observer>>>();
	static std::shared_ptr<Command> _main_handler;

	class User {
		
		bool _main = true;
	public:
		std::string _str = "";
		std::unique_ptr<Command> _command_handler;
				
		User(std::size_t n){
			_command_handler = std::make_unique<Command>(n, observers);
			if (observers->empty()) {
				_command_handler->subscribe(std::make_unique<FileObserver>());
				_command_handler->subscribe(std::make_unique<TerminalObserver>());
			}

			if (_main_handler == nullptr) {
				_main_handler = std::make_shared<Command>(n, observers);
			}

			_command_handler->set_mode(false);
		}

		bool is_main() const{
			return _main;
		}

		void set_command_mode(bool b) {
			_main = b;
		}

		void add_main(const std::string& s) {
			_main_handler->add_command(s, std::ref(this->_main));
		}

		void add_personal(const std::string& s) {
			_command_handler->add_command(s, std::ref(this->_main));
		}

		
	};

	
	static std::size_t user_id;
	std::map<int, std::shared_ptr<User>> users;


	handle_t connect(std::size_t bulk) {
		++user_id;
		users.emplace(std::make_pair(user_id, std::make_shared<User>(bulk)));
		return reinterpret_cast<handle_t>(user_id);
	}


	void receive(handle_t handle, const char *data, std::size_t size) {
		std::size_t id = reinterpret_cast<std::size_t>(handle);
		auto us = users.find(id);
		if (us == users.end()) {
			std::cout << "wrong handle!" << std::endl;
			return;
		}
	
		std::string user_str = us->second->_str;
		user_str.append(data, size);
		int it = user_str.find('\n', 0);
		while (it != user_str.npos) {
			if (users[id]->is_main()) {
				users[id]->add_main(user_str.substr(0, it));
			}
			else {
				users[id]->add_personal(user_str.substr(0, it));
			}
			user_str.erase(0, it + 1);
			it = user_str.find('\n', 0);
		}
		us->second->_str = user_str;
	}

	void disconnect(handle_t handle) {
		std::size_t id = reinterpret_cast<std::size_t>(handle);
		users.erase(id);
	}

}
