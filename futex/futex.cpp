#include <cstdlib>
#include <cstdio>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

int d = 0;

class futex {
public:
	std::atomic<int> interlock;
	bool lock(int numberOfThread) {
		int desired = 0;
		while (!(interlock.compare_exchange_strong(desired, numberOfThread))) {
			desired = 0;
		}
		return true;
	}
	void unlock(int numberOfThread) {
		int desired = 0;
		bool unlocked = interlock.compare_exchange_strong(numberOfThread, desired);
		assert(unlocked);
		
		//if (desired != 0) {
		//	std::cout << desired;
		//}

	}
};

bool increment(volatile long long & b, long long needed, futex & mutex, int i) {
	while (true) {
		mutex.lock(i);
		if (b == needed) {
			mutex.unlock(i);
			return 1;
		}
		b++;
		mutex.unlock(i);

	}
}

int main() {
	futex mutex;
	volatile long long a = 0;
	long long need = 10000000000;
	std::vector<std::thread> threads;
	for (int i = 1; i < 11; ++i) {
		threads.push_back(std::thread(increment, std::ref(a), need, std::ref(mutex), i));
	}

	for (std::vector<std::thread>::iterator i = threads.begin(); i < threads.end(); ++i) {
		(*i).join();
	}

	std::cout << a;
	return 0;
}