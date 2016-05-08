#include "lock_free.hpp"
#include <thread>
//#define BOOST_TEST_MODULE MyTest
//#define BOOST_TEST_MAIN
//#include <boost/test/unit_test.hpp>

void push(lock_free_stack<int> & stack, std::atomic<int> & size) {
	for (int i = 0; i < 100000; ++i) {
		stack.push(rand() % 100);
		size++;
	}
}

void pop(lock_free_stack<int> & stack, std::atomic<int> & size) {
	for (int i = 0; i < 100000; ++i) {
		stack.pop();
		size--;
	}
}



//BOOST_AUTO_TEST_CASE(test)
int main(){
	lock_free_stack<int> stack;
	std::atomic<int> size = 50;

	for (unsigned int i = 0; i < 50; ++i) {
		stack.push(rand() % 100);
	}
	std::vector<std::thread> threads;
	int numberOfThreads = std::thread::hardware_concurrency() / 2;

	for (int i = 0; i < numberOfThreads; ++i) {
		threads.push_back(std::thread(push, std::ref(stack), std::ref(size)));
	}
	for (int i = 0; i < numberOfThreads; ++i) {
		threads.push_back(std::thread(pop, std::ref(stack), std::ref(size)));
	}

	for (std::vector<std::thread>::iterator i = threads.begin(); i < threads.end(); ++i) {
		(*i).join();
	}

	//BOOST_CHECK_EQUAL(size, 50);
	if (size == 50) {
		printf("Everything ok");
	}
	else {
		printf("ABA problem, size = %d", size);
	}
}
