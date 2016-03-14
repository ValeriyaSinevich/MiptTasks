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
bool increment(volatile long long & b, long long needed, futex & ftx, int i);

bool increment(volatile long long & b, long long needed, std::mutex & mtx, int i);
