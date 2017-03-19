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
    std::vector<std::experimental::filesystem::path> dirs;
    std::vector<std::string> files;
    std::string currentDir;
    Result (Result && res) 
        : dirs(std::move(res.dirs))
        , files(std::move(res.files))
        , currentDir(std::move(res.currentDir))
    {}

    Result()
    {}

    Result& operator=(Result&& other)
    {
        dirs = std::move(other.dirs);
        files = std::move(other.files);
        currentDir = std::move(other.currentDir);

        return *this;
    }

    Result& operator=(const Result& other)
    {
        if (this != &other)
        {
            dirs = other.dirs;
            files = other.files;
            currentDir = other.currentDir;
        }
        return *this;
    }
};

Result listDir(std::experimental::filesystem::path && path)
{
    Result res;

    res.currentDir = path.string();

    for (auto &element : std::experimental::filesystem::directory_iterator(path))
    {
        if (std::experimental::filesystem::is_directory(element))
        {
            res.dirs.emplace_back(element.path());
        }
        else
        {
            res.files.emplace_back(element.path().filename().string());
        }
    }

    return res;
}

int main()
{
    std::string root = "C:f";

    std::stack<std::experimental::filesystem::path> dirsToDo;
    std::stack<std::string> files;

    dirsToDo.push(root);

    const unsigned int HC = std::thread::hardware_concurrency() * 2;

    while (!dirsToDo.empty())
    {
        std::stack<std::future<Result>> futures;
        for (unsigned int i = 0; i < HC && !dirsToDo.empty(); ++i)
        {
            auto ftr = std::async(std::launch::async, &listDir, std::move(dirsToDo.top()));
            dirsToDo.pop();

            futures.emplace(std::move(ftr));
        }
        try
        {

            while (!futures.empty())
            {
                auto ftr = std::move(futures.top());
                futures.pop();

                Result res = std::move(ftr.get());

                res.files.emplace_back(res.currentDir);

                for (auto &file : res.files)
                {
                    files.emplace(file);
                }

                for (auto &dir : res.dirs)
                {
                    dirsToDo.emplace(dir);
                }
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

    while (!files.empty())
    {
        std::cout << files.top() << "\n";
        files.pop();
    }

    return 0;
}