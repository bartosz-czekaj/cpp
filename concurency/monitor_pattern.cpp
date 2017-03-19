#include <thread>
#include <string>
#include <iostream>
#include <chrono>
#include <future>
#include <vector>
#include <iterator>
#include <filesystem>
#include <stack>


struct Result
{
    std::stack<std::experimental::filesystem::path> dirs;
    std::stack<std::string> files;

    Result (Result && res) 
        : dirs(std::move(res.dirs))
        , files(std::move(res.files))
    {}

    Result()
    {}

    Result& operator=(Result&& other)
    {
        dirs = std::move(other.dirs);
        files = std::move(other.files);

        return *this;
    }

    Result& operator=(const Result& other)
    {
        if (this != &other)
        {
            dirs = other.dirs;
            files = other.files;
        }
        return *this;
    }
};


class MonitorResult
{
public:
    void PutFile(std::string && file)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _result.files.emplace(file);
    }

    void PutDir(const std::experimental::filesystem::path & path)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _result.dirs.emplace(path);
    }

    bool IsDirEmpty()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _result.dirs.empty();
    }

    std::stack<std::string> &GetFiles()
    {
        return _result.files;
    }

    std::stack<std::experimental::filesystem::path> GetDirs(int n)
    {
        std::stack<std::experimental::filesystem::path> dirs;

        std::lock_guard<std::mutex> lock(_mutex);

        for (int i = 0; i < n && !_result.dirs.empty(); ++i)
        {
            dirs.emplace(std::move(_result.dirs.top()));
            _result.dirs.pop();
        }

        return dirs;
    }

private:
    std::mutex _mutex;
    Result _result;
};

void listDir(std::experimental::filesystem::path && path, MonitorResult & result)
{
    result.PutFile(path.string());

    for (auto &element : std::experimental::filesystem::directory_iterator(path))
    {
        if (std::experimental::filesystem::is_directory(element))
        {
            result.PutDir(element.path());
        }
        else
        {
            result.PutFile(element.path().filename().string());
        }
    }
}

int main()
{
    auto startTime = std::chrono::system_clock::now();

    std::string root = "C:\\";

    MonitorResult result;

    result.PutDir(root);

    const unsigned int HC = std::thread::hardware_concurrency() * 8;

    while (!result.IsDirEmpty())
    {
        std::stack<std::experimental::filesystem::path> dirsToDo = result.GetDirs(HC);
        std::stack<std::future<void>> futures;

        while (!dirsToDo.empty())
        {
            auto ftr = std::async(std::launch::async, &listDir, std::move(dirsToDo.top()), std::ref(result));
            dirsToDo.pop();

            futures.emplace(std::move(ftr));
        }
        try
        {

            while (!futures.empty())
            {
                auto ftr = std::move(futures.top());
                futures.pop();
                ftr.wait();
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Standard exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "Unknown exception" << std::endl;
        }
    }

    auto files = result.GetFiles();

    auto stopTime = std::chrono::system_clock::now();

    std::cout << "Execution time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime)).count() << std::endl;

    return 0;
}