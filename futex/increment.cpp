#include "futexHeader.h"

bool incrementFtx(volatile long long & b, long long needed, futex & ftx, int i) {
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

bool incrementMtx(volatile long long & b, long long needed, std::mutex & mtx, int i) {
	while (true) {
		mtx.lock();
		if (b == needed) {
			mtx.unlock();
			return 1;
		}
		b++;
		mtx.unlock();

	}
}


double incrementFutex(int numberOfThreads) {
	futex ftx;
	double duration;
	volatile long long a = 0;
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

double incrementMutex(int numberOfThreads) {
	std::mutex mtx;
	double duration;
	volatile long long a = 0;
	long long need = 1000000;
	std::vector<std::thread> threads;
	std::clock_t start;
	start = std::clock();
	for (int i = 1; i < numberOfThreads; ++i) {
		threads.push_back(std::thread(incrementMtx, std::ref(a), need, std::ref(mtx), i));
	}

	for (std::vector<std::thread>::iterator i = threads.begin(); i < threads.end(); ++i) {
		(*i).join();
	}
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << a << std::endl;
	return duration;
}

void launch(int numberOfThreads, std::ofstream & out) {
	out << "number of threads is  " << numberOfThreads << std::endl;
	double futexDuration = incrementFutex(numberOfThreads);
	double mutexDuration = incrementMutex(numberOfThreads);
	out << std::endl << "futex: " << futexDuration << std::endl << "mutex: " << mutexDuration << std::endl;
	if (futexDuration < mutexDuration)
		out << "custom futex is " << mutexDuration / futexDuration << " times faster" << std::endl << std::endl << std::endl;
	else
		out << "default mutex is " << futexDuration / mutexDuration << " times faster" << std::endl << std::endl << std::endl;
}

int main() {
	std::ofstream out;
	out.open("futexVSmutex.out", std::ios::ate);
	launch(11, out);
	launch(std::thread::hardware_concurrency() / 2, out);
	launch(std::thread::hardware_concurrency() *2, out);
	return 0;
}