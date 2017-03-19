#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
	std::cout << "write_x\n";
	x.store(true, std::memory_order_seq_cst);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
void write_y()
{
	std::cout << "write_y\n";
	y.store(true, std::memory_order_seq_cst);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
void read_x_then_y()
{
	std::cout << "read_x_then_y 1\n";
	while (!x.load(std::memory_order_seq_cst));
	std::cout << "read_x_then_y 2\n";
	if (y.load(std::memory_order_seq_cst)) {
		std::cout << "read_x_then_y 3\n";
		++z;
	}
}
void read_y_then_x()
{
	std::cout << "read_y_then_x 1\n";
	while (!y.load(std::memory_order_seq_cst));
	std::cout << "read_y_then_x 2\n";
	if (x.load(std::memory_order_seq_cst)) {
		std::cout << "read_y_then_x 3\n";
		++z;
	}
}
int main()
{
	x = false;
	y = false;
	z = 0;
	std::thread a(write_x);
	std::thread c(read_y_then_x);
	std::thread b(write_y);
	std::thread d(read_x_then_y);

	a.join();
	b.join();
	c.join();
	d.join();

	std::cout << "z: " << z;

	assert(z.load() != 0);
}
