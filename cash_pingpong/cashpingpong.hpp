#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <string>

#define changing_iterations 100000000

void change(long long & x) {
	for (int i = 0; i < changing_iterations; ++i)
		x++;
}

struct DifferentCacheLine {
	long long store1;
	int store[16]; 
	long long store2;

	DifferentCacheLine() : store1(0), store2(0) {}
};

struct SameCacheLine {
	long long store1;
	long long store2;

	SameCacheLine() : store1(0), store2(0) {}
};

template<class T>
void changeVariables(T & a, std::string cash_line_name) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	std::vector<std::thread> threads;
	threads.push_back(std::thread(change, std::ref(a.store1)));
	threads.push_back(std::thread(change, std::ref(a.store1)));
	threads[0].join();
	threads[1].join();
	end = std::chrono::system_clock::now();
	int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << cash_line_name << " works for " << time << " milliseconds\n";

}

int main() {
	SameCacheLine line1;
	DifferentCacheLine line2;

	changeVariables(line1, "same_cache_line");
	changeVariables(line2, "different_cache_line");

	return 0;
}