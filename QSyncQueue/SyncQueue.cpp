
#include <iostream>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <cstdio>

const int number_of_tasks = 1024;

template <template <typename Type> class Container_type, typename Type>
class CSyncQueueContainer {
	Container_type<element_type> container();
	int container_size = 0;
	bool firtElementToBePopped = false;

	std::unique_lock<std::mutex> push_lock();
	std::condition_variable push_cv;

	std::unique_lock<std::mutex> pop_lock();
	std::condition_variable pop_cv;

	std::unique_lock<std::mutex> newElemwentWaitingToBePopped_lock();
	std::condition_variable newElemwentWaitingToBePopped_cv;

	std::mutex elementReadyForPop;

	void push();
	void popOrSleep();
	void popNoSleep();
};

template <template <typename Type> class Container_type, typename Type>
void CSyncQueueContainer::push(Type addValue) {
	cv_push.wait(&lock, container_size == number_of_tasks);
	if (container_size == 0) {
		elementReadyForPop = true;
		container.push(addValue);
		cv_pop.notify_one();
	}
	else if (container_size == 1) {
		newElemwentWaitingToBePopped_cv.wait(&newElemwentWaitingToBePopped_lock, elementReadyForPop);
		container.push(addValue);
	}
	container_size++;
}

template <template <typename Type> class Container_type, typename Type>
Type CSyncQueueContainer::popOrSleep() {
	cv_pop.wait(&pop_lock, container_size > 0);
	Type element = container.top();
	container.pop_back();
	container_size--;
	cv_push.notify_one();
	return last_elem;
}

template <template <typename Type> class Container_type, typename Type>
void CSyncQueueContainer::popNoSleep() {
	pop_lock.lock();
	if (container_size() == 0)
		std::cout << "no element in container" << std::endl;
	else {
		container.pop_back();
		container_size--;
		cv_push.notify_one();
	}
	pop_lock.unlock();
}

int main() {
}