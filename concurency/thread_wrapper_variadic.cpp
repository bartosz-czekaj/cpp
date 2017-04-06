#include <iostream>
#include <string>
#include <list>
#include <shared_mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>

class ThreadWrapper
{
private:

	std::exception_ptr mExceptionPtr;
	std::thread mThread;

	bool Joinable() const noexcept
	{
		return mThread.joinable();
	}

public:

	ThreadWrapper() noexcept = delete;

	ThreadWrapper(ThreadWrapper &&t) noexcept
		: mExceptionPtr(std::move(t.mExceptionPtr))
		, mThread(std::move(t.mThread))
	{}

	ThreadWrapper &operator =(ThreadWrapper &&t) noexcept
	{
		mExceptionPtr = std::move(t.mExceptionPtr);
		mThread = std::move(t.mThread);
		return *this;
	}

	template<typename Callable, typename... Args>
	ThreadWrapper(Callable &&f, Args &&... args)
		: mExceptionPtr(nullptr)
		, mThread([&](typename std::decay<Callable>::type &&f, typename std::decay<Args>::type &&... args)
	{
		try
		{
			std::bind(f, args...)();
		}
		catch (...)
		{
			mExceptionPtr = std::current_exception();
		}

	}, std::forward<Callable>(f), std::forward<Args>(args)...)
	{	}

	~ThreadWrapper() noexcept
	{
		Join();
	}

	void Join()
	{
		if (Joinable())
		{
			mThread.join();
		}

		if (mExceptionPtr != nullptr)
		{
			std::rethrow_exception(mExceptionPtr);
		}
	}

	void Detach()
	{
		if (Joinable())
		{
			mThread.detach();
		}
	}

	std::thread::id GetId() const noexcept
	{
		return mThread.get_id();
	}

	std::thread::native_handle_type NativeHandle()
	{
		return mThread.native_handle();
	}
};

void thFun(int idx, std::string text)
{
	for (int i = 0; i < idx; ++i)
	{
		std::cout << (i + 1) << " "<<text << std::endl;
	}
}

int main()
{
	ThreadWrapper th(thFun, 20, "sample");
	th.Join();

	return 0;
}