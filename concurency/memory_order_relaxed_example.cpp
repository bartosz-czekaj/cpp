#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

//Atomic operations tagged memory_order_relaxed are not synchronization operations, they do not order memory. They only guarantee atomicity and modification order consistency.
std::atomic<int> cnt = { 0 };

void f()
{
	for (int n = 0; n < 1000; ++n) 
	{
		//Typical use for relaxed memory ordering is incrementing counters
		cnt.fetch_add(1, std::memory_order_relaxed);
	}
}

int main()
{
	std::vector<std::thread> v;
	for (int n = 0; n < std::thread::hardware_concurrency()*2; ++n) {
		v.emplace_back(f);
	}
	for (auto& t : v) {
		t.join();
	}
	std::cout << "Final counter value is " << cnt << '\n';

	return 0;
}