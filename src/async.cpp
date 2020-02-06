#include <map>
#include <algorithm>
#include "async.h"
#include "command.h"

namespace async {
	
	obs_vec_ptr observers = std::make_shared<std::vector<std::unique_ptr<Observer>>>();

	class User {
	public:
		User(std::size_t n){
			command_handler = std::make_unique<Command>(n, observers);
			if (observers->empty()) {
				command_handler->subscribe(std::make_unique<FileObserver>());
				command_handler->subscribe(std::make_unique<TerminalObserver>());
			}
		}

		std::string str ="";
		std::unique_ptr<Command> command_handler;
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
	std::string user_str = users[id]->str;
	user_str.append(data, size);
	int it = user_str.find('\n', 0);
	while (it != user_str.npos) {
		users[id]->command_handler->add_command(user_str.substr(0, it));
		user_str.erase(0, it + 1);
		it = user_str.find('\n', 0);
	}
	users[id]->str = user_str;
}

void disconnect(handle_t handle) {
	std::size_t id = reinterpret_cast<std::size_t>(handle);
	users.erase(id);
}

}
