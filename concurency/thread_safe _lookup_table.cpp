#include <iostream>
#include <string>
#include <list>
#include <shared_mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

template<typename Key, typename Value, typename Hash = std::hash<Key> >
class threadsafe_lookup_table
{
private:
	class bucket_type
	{
	private:
		using bucket_value = std::pair<Key, Value>;
		using bucket_data = std::list<bucket_value>;

		typedef typename bucket_data::iterator bucket_iterator;

		bucket_data data;
		std::shared_mutex mutex;
		bucket_iterator find_entry_for(const Key &key)
		{
			return std::find_if(data.begin(), data.end(),
				[&](const auto &item)
			{
				return item.first == key;
			});
		}

	public:
		Value value_for(const Key &key)
		{
			std::shared_lock<std::shared_mutex> lock(mutex);
			auto found_entry = find_entry_for(key);
			return (found_entry == data.end()) ? Value() : found_entry->second;
		}

		void add_or_update_mapping(const Key &key, const Value &value)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);

			auto found_entry = find_entry_for(key);

			if (found_entry == data.end())
			{
				data.push_back(bucket_value(key, value));
			}
			else
			{
				found_entry->second = value;
			}
		}

		void remove_mapping(const Key &key)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			bucket_iterator const found_entry = find_entry_for(key);
			if (found_entry != data.end())
			{
				data.erase(found_entry);
			}
		}

		void get_map(std::map<Key, Value> &retmap)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			for (const auto &pair : data)
			{
				retmap.insert(pair);
			}
		}
	};

	std::vector<std::unique_ptr<bucket_type> > buckets;
	Hash hasher;
	bucket_type& get_bucket(const Key &key) const
	{
		std::size_t const bucket_index = hasher(key) % buckets.size();
		return *buckets[bucket_index];
	}
public:

	threadsafe_lookup_table(unsigned num_buckets = 23/*prime number*/, Hash const& hasher_ = Hash())
		: buckets(num_buckets)
		, hasher(hasher_)
	{

		for (auto &bucket : buckets)
		{
			bucket.reset(new bucket_type);
		}
	}

	threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
	threadsafe_lookup_table& operator=(threadsafe_lookup_table const& other) = delete;

	Value value_for(Key const& key) const
	{
		return get_bucket(key).value_for(key);
	}

	void add_or_update_mapping(const Key &key, const Value &value)
	{
		get_bucket(key).add_or_update_mapping(key, value);

	}
	void remove_mapping(const Key &key)
	{
		get_bucket(key).remove_mapping(key);
	}

	std::map<Key, Value> get_map() const
	{
		std::map<Key, Value> res;

		for (auto &bucket : buckets)
		{
			bucket->get_map(res);
		}

		return res;
	}
};



int main()
{
	threadsafe_lookup_table<std::string, int> lookupTable;

	std::thread t1([&]()
	{
		for (int i = 0; i < 30; ++i)
		{
			if (i % 2 != 0)
			{
				lookupTable.add_or_update_mapping(std::to_string(i), i);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});

	std::thread t2([&]()
	{
		for (int i = 0; i < 30; ++i)
		{
			if (i % 2 == 0)
			{
				lookupTable.add_or_update_mapping(std::to_string(i), i);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});

	std::thread t3([&]()
	{
		std::cout << "*********************************************\n";
		for (const auto &pair : lookupTable.get_map())
		{
			std::cout << pair.first << " " << pair.second << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	});

	t1.join();
	t2.join();
	t3.join();

	return 0;
}