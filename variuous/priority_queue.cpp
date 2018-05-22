class Heap
{
private:
	std::priority_queue<int, std::vector<int>, std::less<int> > qLower;
	std::priority_queue<int, std::vector<int>, std::greater<int> > qGreater;

	void balance()
	{
		while (!qLower.empty() && !qGreater.empty() && qLower.top() > qGreater.top())
		{
			int lowHead = qLower.top();
			int highHead = qGreater.top();
			qLower.pop();
			qGreater.pop();
			qLower.emplace(highHead);
			qGreater.emplace(lowHead);
		}
	}

public:
	void add(int number)
	{

        if (qLower.size() <= qGreater.size())
        {
            qLower.emplace(number);
        }
        else
        {
            qGreater.emplace(number);
        }
    	balance();
	}

	double median()
	{
		if (!qLower.empty() && qGreater.empty())
		{
			return qLower.top();
		}
		if (!qLower.empty() && !qGreater.empty())
		{
			return qLower.size() == qGreater.size() ? (qLower.top() + qGreater.top()) / 2.0 : qLower.top();
		}
		return 0.0;
	}
};