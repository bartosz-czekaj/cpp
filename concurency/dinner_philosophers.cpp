#include "stdafx.h"
#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <memory>

std::mutex g_lockprint;
constexpr  int no_of_philosophers = 6;

struct fork
{
	std::mutex mutex;
};

struct table
{
	std::atomic<bool>                ready{ false };
	std::vector<fork>				 forks;

	table(int iforks)
	:forks(std::move(std::vector<fork>(iforks)))
	{
	}
};

struct philosopher
{
private:
	const std::string	name;
	table			   &dinnertable;
	fork               &left_fork;
	fork 			   &right_fork;
	std::thread			lifethread;
	std::mt19937		rng{ std::random_device{}() };

	std::atomic_flag					 atomic_flag2 = ATOMIC_FLAG_INIT;
public:
	philosopher(const std::string &name, table & table, fork & left_fork, fork & right_fork) 
		:name(name)
		,dinnertable(table)
		,left_fork(left_fork)
		,right_fork(right_fork)
		,lifethread(&philosopher::dine, this)
	{
	}

	philosopher(philosopher&& o) noexcept 
		: name(std::move(o.name))
		, dinnertable(std::move(o.dinnertable))
		, left_fork(std::move(o.left_fork))
		, right_fork(std::move(o.right_fork))
		, lifethread(std::move(o.lifethread))
	{
	
	}


	~philosopher()
	{
		lifethread.join();
	}

	void dine()
	{
		while (!dinnertable.ready);
		do
		{
			think();
			eat();
		} while ( dinnertable.ready);
	}

	void print(const std::string &text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockprint);
		//std::atomic_thread_fence(std::memory_order_acquire);
		std::cout<< std::left << std::setw(10) << std::setfill(' ')<< name << text << std::endl;
		//std::atomic_thread_fence(std::memory_order_release);
	}

	void eat()
	{
		std::lock(left_fork.mutex, right_fork.mutex);

		std::lock_guard<std::mutex> left_lock(left_fork.mutex, std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);

		print(" started eating.");

		static thread_local std::uniform_int_distribution<> dist(1, 10);//generates random number
		std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

		print(" finished eating.");
	}

	void think()
	{
		static thread_local std::uniform_int_distribution<> wait(1, 10);//generates random number
		std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));

		print(" is thinking ");
	}
};

void dine()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Dinner started!" << std::endl;

	{
		table table(6);

		std::vector<std::unique_ptr<philosopher>> philosophers;

		philosophers.emplace_back(std::make_unique<philosopher>("Aristotle", table, table.forks[0], table.forks[1]));
		philosophers.emplace_back(std::make_unique<philosopher>("Platon", table, table.forks[1], table.forks[2]));
		philosophers.emplace_back(std::make_unique<philosopher>("Descartes", table, table.forks[2], table.forks[3]));
		philosophers.emplace_back(std::make_unique<philosopher>("Kant", table, table.forks[3], table.forks[4]));
		philosophers.emplace_back(std::make_unique<philosopher>("Nietzsche", table, table.forks[4], table.forks[5]));
		philosophers.emplace_back(std::make_unique<philosopher>("Kazio", table, table.forks[5], table.forks[0]));


		table.ready.store(true, std::memory_order_relaxed);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		table.ready.store(false, std::memory_order_release);
	}
	std::cout << "Dinner done!" << std::endl;
}

int main()
{
	dine();

	return 0;
} 