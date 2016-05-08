#include <cstdlib>
#include <cstdio>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <ctime>
#include <mutex>
#include <fstream>
#include <atomic>
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

#define max_value 10000000

struct not_aligned {
	std::atomic<char> a;
	std::atomic<int> store;

	not_aligned() : a('0'), store(0) {};
};

struct aligned {
	std::atomic<int> store;
	std::atomic<char> a;

	aligned() : a('0'), store(0) {};
};

#pragma pack(1)
struct cross_cache_line {
	char a[62];
	std::atomic<int> store;

	cross_cache_line() : store(0) {}
};

void interlocked(std::atomic<int>* x) {
	while (*x <= max_value) {
		x->fetch_add(1);
	}
}

int increment(std::atomic<int>* a){
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	std::vector<std::thread> threads;

	for (unsigned int i = 1; i < std::thread::hardware_concurrency() / 2; ++i) {
		threads.push_back(std::thread(interlocked, a));
	}
	for (std::vector<std::thread>::iterator i = threads.begin(); i < threads.end(); ++i) {
		(*i).join();
	}

	end = std::chrono::system_clock::now();
	int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	return duration;
}


int main() {
	aligned a;
	not_aligned b;
	cross_cache_line c;

	int aligned_duration = increment(&a.store); //2632 milliseconds
	int not_aligned_duration = increment(&b.store); //3036 milliseconds
	int cross_cache_duration = increment(&c.store); //3358 milliseconds

	std::cout << std::endl << "interlocked duration with aligning: " << aligned_duration << std::endl
		<< "interlocked duration without aligning: " << not_aligned_duration << std::endl
		<< "interlocked duration with atomic crossing cacheline border: " << cross_cache_duration << std::endl;
	return 0;
}
