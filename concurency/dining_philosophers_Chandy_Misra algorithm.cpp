#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <iomanip>
#include <condition_variable>
#include <map>
#include <vector>


std::mutex g_lockprint;
std::map<std::string, int> data;

class sync_channel
{
	std::mutex              mutex;
	std::condition_variable cv;

public:

	void wait()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock);
	}

	void notifyall()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.notify_all();
	}
};



class fork
{
	int            id;
	int            owner;
	bool           dirty;
	std::mutex     mutex;
	sync_channel   channel;

public:

	fork(int const forkId, int const ownerId) 
		: id(forkId)
		, owner(ownerId)
		, dirty(true)
	{}

	void request(int const ownerId)
	{
		while (owner != ownerId)
		{
			if (dirty)
			{
				std::lock_guard<std::mutex> lock(mutex);

				dirty = false;
				owner = ownerId;
			}
			else
			{
				channel.wait();
			}
		}
	}

	void done_using()
	{
		dirty = true;
		channel.notifyall();
	}

	std::mutex& getmutex() { return mutex; }
};

struct table_setup
{
	std::atomic<bool> done{ false };
	sync_channel      channel;
	std::vector<std::unique_ptr<fork>> forks;


	table_setup(int iforks)
	{
		for (int i = 0; i < iforks; ++i)
		{
			forks.emplace_back(std::move(new fork(i, i)));
		}
	}

};

struct philosopher
{
private:
	int               id;
	std::string const name;
	table_setup&      setup;
	int             left_fork_id;
	int             right_fork_id;
	std::thread       lifethread;
public:
	philosopher(int const id, std::string const & n, table_setup & s, int lf_id, int rf_id) :
		id(id), name(n), setup(s), left_fork_id(lf_id), right_fork_id(rf_id), lifethread(&philosopher::dine, this)
	{
	}

	philosopher(philosopher &&ph)
		:id(std::move(ph.id))
		, name(std::move(ph.name))
		, setup(std::move(ph.setup))
		, left_fork_id(std::move(ph.left_fork_id))
		, right_fork_id(std::move(ph.right_fork_id))
		, lifethread(std::move(ph.lifethread))
	{
	}

	~philosopher()
	{
		lifethread.join();
	}

	void dine()
	{
		setup.channel.wait();

		do
		{
			think();
			eat();
		} while (!setup.done);
	}

	void print(std::string const & text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockprint);
		/*std::cout
			<< std::left << std::setw(10) << std::setfill(' ')
			<< name << text << std::endl;*/

		data[text]++;

	}

	void eat()
	{
		setup.forks[left_fork_id]->request(id);
		setup.forks[right_fork_id]->request(id);

		std::lock(setup.forks[left_fork_id]->getmutex(), setup.forks[right_fork_id]->getmutex());

		std::lock_guard<std::mutex> left_lock(setup.forks[left_fork_id]->getmutex(), std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(setup.forks[right_fork_id]->getmutex(), std::adopt_lock);

		print(name + " started eating.");
		//print(" finished eating.");

		setup.forks[left_fork_id]->done_using();
		setup.forks[right_fork_id]->done_using();
	}

	void think()
	{
		print(name + " is thinking ");
	}
};

class table
{
	const static int no_of_philosophers = 7;

	table_setup setup{ no_of_philosophers };
	std::vector<std::unique_ptr<philosopher>> p;
	
public:

	table()
	{
		p.emplace_back(std::move(new philosopher(1, "Aristotle", setup, 0, 1 )));
		p.emplace_back(std::move(new philosopher(2, "Platon",	 setup, 1, 2)));
		p.emplace_back(std::move(new philosopher(3, "Descartes", setup, 2, 3)));
		p.emplace_back(std::move(new philosopher(4, "Kant",      setup, 3, 4)));
		p.emplace_back(std::move(new philosopher(5, "Nietzsche", setup, 4, 5)));
		p.emplace_back(std::move(new philosopher(6, "Hume",      setup, 5, 6)));
		p.emplace_back(std::move(new philosopher(7, "Russell",   setup, 6, 0)));

	}

	void start()
	{
		setup.channel.notifyall();
	}

	void stop()
	{
		setup.done = true;
	}
};

void dine()
{
	std::cout << "Dinner started!" << std::endl;

	{
		table table;

		table.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		table.stop();
	}

	std::cout << "Dinner done!" << std::endl;
}

int main()
{
	dine();

	for (const auto &d : data)
	{
		std::cout << d.first << ": " << d.second << std::endl;
	}

	return 0;
}