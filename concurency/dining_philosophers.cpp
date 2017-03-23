#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <map>

std::mutex print_mutex;
std::map<std::string, int>  all;

struct table
{
	std::atomic<bool>                ready{ false };
	std::vector<std::mutex>			 forks;

	table(int iforks)
		:forks(std::move(std::vector<std::mutex>(iforks)))
	{
	}
};

struct philosopher
{
private:
	const std::string	name;
	table			  &dinnertable;
	int                left_fork_id;
	int 			   right_fork_id;
	std::thread			philosopher_thread;
	std::mt19937		rng{ std::random_device{}() };

public:
	philosopher(const std::string &name, table & table, int left_fork, int  right_fork)
		: name(name)
		, dinnertable(table)
		, left_fork_id(left_fork)
		, right_fork_id(right_fork)
		, philosopher_thread(&philosopher::dinner, this)
	{
	}

	void dinner()
	{
		while (!dinnertable.ready);

		do
		{
			think();
			eat();
		} while (dinnertable.ready);
	}

	void think()
	{
		static thread_local std::uniform_int_distribution<> wait(1, 10);//generates random number
		std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 50));

		print(name + " thinking.");
	}

	void eat()
	{
		std::lock(dinnertable.forks[left_fork_id], dinnertable.forks[right_fork_id]);
		std::lock_guard<std::mutex> lock_a(dinnertable.forks[left_fork_id], std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(dinnertable.forks[right_fork_id], std::adopt_lock);

		static thread_local std::uniform_int_distribution<> wait(1, 10);//generates random number
		std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 50));

		print(name + " eating.");

	}

	void print(const std::string &text)
	{
		std::lock_guard<std::mutex> lock_print(print_mutex);
		//std::cout << text << std::endl;

		all[text]++;

	}

	~philosopher()
	{
		philosopher_thread.join();
	}
};

void phdinner()
{
	table table(7);

	std::vector<std::unique_ptr<philosopher>> philosophers;

	philosophers.emplace_back(std::make_unique<philosopher>("P1", table, 0, 1));
	philosophers.emplace_back(std::make_unique<philosopher>("P2", table, 1, 2));
	philosophers.emplace_back(std::make_unique<philosopher>("P3", table, 2, 3));
	philosophers.emplace_back(std::make_unique<philosopher>("P4", table, 3, 4));
	philosophers.emplace_back(std::make_unique<philosopher>("P5", table, 4, 5));
	philosophers.emplace_back(std::make_unique<philosopher>("P6", table, 5, 6));
	philosophers.emplace_back(std::make_unique<philosopher>("P7", table, 6, 0));


	table.ready.store(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(2500));
	table.ready.store(false);
}

int main()
{
	
	{
		phdinner();
	}

	for (const auto &item : all)
	{
		std::cout << item.first << ": " << item.second << std::endl;
	}

	return 0;
} 