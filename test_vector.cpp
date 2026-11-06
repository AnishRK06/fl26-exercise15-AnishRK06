// test_vector.cpp
// Client code to test a thread-safe vector
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

#include "thread_safe_vector.hpp"

ThreadSafeVector<int> safeVector;

void pushToVector()
{
	for (int i = 0; i < 10; i++) {
		safeVector.push_back(i);
	}
}

void popFromVector()
{
	int popped_value = -1;
	for (int i = 0; i < 5; i++) {
		safeVector.wait_and_pop_back(popped_value);
		std::cout << popped_value;
	}
}


int main() 
{
	std::thread t1(pushToVector);
	std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
	std::thread t2(popFromVector);
	std::thread t3(popFromVector);

	t1.join();
	t2.join();
	t3.join();

	std::cout << std::endl;
	
	return EXIT_SUCCESS;
}
