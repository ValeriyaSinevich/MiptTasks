#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <string>


#include <stdlib.h>
#include <windows.h>

size_t cache_line_size() {
	size_t line_size = 0;
	DWORD buffer_size = 0;
	DWORD i = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

	GetLogicalProcessorInformation(0, &buffer_size);
	buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
	GetLogicalProcessorInformation(&buffer[0], &buffer_size);

	for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
		if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
			line_size = buffer[i].Cache.LineSize;
			break;
		}
	}

	free(buffer);
	return line_size;
}


#define changing_iterations 100000000

void change(long long & x) {
	for (int i = 0; i < changing_iterations; ++i)
		x++;
}


struct different_cache_line {
	long long store1;
	int store[16];
	long long store2;

	different_cache_line() : store1(0), store2(0) {}
};

struct same_cache_line {
	long long store1;
	long long store2;

	same_cache_line() : store1(0), store2(0) {}
};

template<class T>
void increment(T & a, std::string cash_line_name) {
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
	same_cache_line line1;
	different_cache_line line2;

	std::cout << cache_line_size() << std::endl;

	increment(line1, "same_cache_line"); //3596 milliseconds
	increment(line2, "different_cache_line"); //4378 milliseconds

	return 0;
}