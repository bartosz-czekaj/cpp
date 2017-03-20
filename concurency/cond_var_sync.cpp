std::condition_variable cv_setup_done;

void worker_thread(bool &iDone)
{
    std::cout << "WORKER: Starting setup..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "WORKER: Setup done. Notifying main thread." << std::endl;
    iDone = true;
    cv_setup_done.notify_one();
    std::cout << "WORKER: Notified main thread." << std::endl;
}



int main()
{
    bool vDone = false;

    std::cout << "MAIN: Launching worker thread" << std::endl;
    std::thread wt = std::thread(worker_thread, std::ref(vDone));
    std::mutex vMutex;

    std::unique_lock<std::mutex> locker(vMutex);
    cv_setup_done.wait(locker, [&]() {
        return vDone;
    });

    std::cout << "MAIN: Setup done." << std::endl;

    wt.join();

    return 0;
}