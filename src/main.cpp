#include <iostream>
#include <thread>
#include <vector>

#include "async.h"

int main(int, char* []) {

	std::size_t bulk = 3;
	/*
	auto h = async::connect(bulk);
	auto h2 = async::connect(bulk);
	auto h3 = async::connect(bulk);
	async::receive(h, "1", 1);
	async::receive(h2, "1\n", 2);
	async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
	async::receive(h, "b\nc\nd\n}\n89\n", 11);
	async::receive(h2, "10\n", 3);
	async::receive(h3, "11\n", 3);
	async::receive(h2, "2\n3\n4\n5\n6\n{\na\n", 14);
	async::receive(h2, "b\nc\nd\n}\n89\n", 11);
	async::receive(h3, "13\n", 3);
	async::disconnect(h);
	async::disconnect(h2);
	async::disconnect(h3);
	*/
	std::vector<std::thread> thread_vec;

		thread_vec.push_back(std::thread([bulk](){
			auto h = async::connect(bulk);
			async::receive(h, "1", 1);
			async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
			async::receive(h, "b\nc\nd\n}\n89\n", 11);
			async::disconnect(h);
		}));

		thread_vec.push_back(std::thread([bulk]() {
			auto h2 = async::connect(bulk);
			async::receive(h2, "1\n", 2);
			async::receive(h2, "10\n", 3);
			async::receive(h2, "2\n3\n4\n5\n6\n{\na\n", 14);
			async::receive(h2, "b\nc\nd\n}\n89\n", 11);
			async::disconnect(h2);
		}));

		thread_vec.push_back(std::thread([bulk]() {
			auto h3 = async::connect(bulk);
			async::receive(h3, "11\n", 3);
			async::receive(h3, "13\n", 3);
			async::disconnect(h3);
			}));
	

		for (auto& v : thread_vec) {
			if (v.joinable())
				v.join();
		}

	return 0;
}
