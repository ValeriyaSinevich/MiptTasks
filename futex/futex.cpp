#include <cstdlib>
#include "futexHeader.h"

bool futex::lock(int numberOfThread) {
	int desired = 0;
	while (!(interlock.compare_exchange_strong(desired, numberOfThread))) {
		desired = 0;
	}
	return true;
}

void futex::unlock(int numberOfThread) {
	int desired = 0;
	bool unlocked = interlock.compare_exchange_strong(numberOfThread, desired);
	assert(unlocked);
}
