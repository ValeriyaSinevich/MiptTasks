#include <cstdlib>
#include <cstdio>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <ctime>
#include <mutex>
#include <fstream>



class futex {
public:
	std::atomic<int> interlock;
	bool lock(int numberOfThread);
	void unlock(int numberOfThread);
};

bool futex::lock(int numberOfThread) {
	int desired = 0;
	while (!(interlock.compare_exchange_strong(desired, numberOfThread, std::memory_order_acquire))) {
		desired = 0;
	}
	return true;
}

void futex::unlock(int numberOfThread) {
	int desired = 0;
	bool unlocked = interlock.compare_exchange_strong(numberOfThread, desired, std::memory_order_release);
	assert(unlocked);
}


class volatile_futex {
public:
	std::atomic<int> interlock;
	bool lock(int numberOfThread);
	void unlock(int numberOfThread);
};

bool volatile_futex::lock(int numberOfThread) {
	int desired = 0;
	while (!(interlock.compare_exchange_strong(desired, numberOfThread, std::memory_order_relaxed))) {
		desired = 0;
	}
	return true;
}

void volatile_futex::unlock(int numberOfThread) {
	int desired = 0;
	bool unlocked = interlock.compare_exchange_strong(numberOfThread, desired, std::memory_order_relaxed);
	assert(unlocked);
}


template<class Futex, typename A>
bool incrementFtx(A & a, long long needed, Futex & ftx, int i) {
	while (true) {
		ftx.lock(i);
		if (b == needed) {
			ftx.unlock(i);
			return 1;
		}
		b++;
		ftx.unlock(i);
	}
}


template<class Futex, typename A>
double launch(int numberOfThreads, A & a) {
	Futex ftx;
	double duration;
	long long need = 1000000;
	std::vector<std::thread> threads;
	std::clock_t start;
	start = std::clock();
	for (int i = 1; i < numberOfThreads; ++i) {
		threads.push_back(std::thread(incrementFtx, std::ref(a), need, std::ref(ftx), i));
	}

	for (std::vector<std::thread>::iterator i = threads.begin(); i < threads.end(); ++i) {
		(*i).join();
	}
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << a << std::endl;
	return duration;
}




int main() {
	volatile long long volatile_a = 0;
	long long a = 0;
	std::cout << "number of threads is  " << std::thread::hardware_concurrency() << std::endl;
	double futex_duration = launch<futex>(std::thread::hardware_concurrency() / 2, a);
	double futex_volatile_duration = launch<volatile_futex>(std::thread::hardware_concurrency() / 2, volatile_a);
	std::cout << std::endl << "futex: " << futex_duration << std::endl << "futex_volatile: " << futex_volatile_duration << std::endl;
}
