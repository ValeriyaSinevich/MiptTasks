#include<set>
#include <cstdlib>
#include <mutex>
#include "lock_free.hpp"

template <typename T>
class memory {
	std::mutex mtx;
	std::set<node<T>*> mem;
	typename std::set<node<T>*>::const_iterator ptr = mem.begin();

	memory() {
		mem.resize(100000);
	}

	node<T>* node_malloc() {
		std::lock_guard<std::mutex> lock(g_i_mutex);
		if (ptr != mem.end()) {
			return *ptr;
		}
		else {
			cout << "no empty memory\n" << endl;
			return 1;
		}
		/*if (ptr->second)
			return ptr->first;
		else {
			while (!ptr->second && ptr != mem.end())
				ptr++;
			if (ptr != mem.end()) {
				ptr->second = false;
				return ptr->first;
				ptr++;
			}
			else {
				cout << "no empty memory\n" << endl;
				return 1;
			}
		}*/
	}

	void node_free(node<T>* a) {
		std::lock_guard<std::mutex> lock(g_i_mutex);
		mem.insert(a);
	}
};