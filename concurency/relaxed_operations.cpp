#include <atomic>
#include <thread>
#include <iostream>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
	x.store(true, std::memory_order_relaxed);
	y.store(true, std::memory_order_relaxed);
	std::cout << "write_x_then_y\n";
}
void read_y_then_x()
{
	std::cout << "read_y_then_x 1\n";
	while (!y.load(std::memory_order_relaxed));
	std::cout << "read_y_then_x 2\n";
	if (x.load(std::memory_order_relaxed)) {
		std::cout << "read_y_then_x 3\n";
		++z;
	}
}
int main()
{
	for (int i = 0; i < 100; i++)
	{
		std::cout << "************************* i: " << i << std::endl;
		x = false;
		y = false;
		z = 0;

		std::thread b(i % 2 ? read_y_then_x : write_x_then_y);
		std::thread a(i % 2 ? write_x_then_y : read_y_then_x);

		a.join();
		b.join();
		assert(z.load() != 0);

		std::cout << "z: " << z << std::endl;
	}

	return 0;
}