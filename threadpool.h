#include <iostream>
#include <mutex>
#include <functional>
#include <vector>
#include <atomic>
#include <thread>
#include <sstream>
#include "blocking_queue.h"


class ThreadPool {

    public:
        ThreadPool(int num_threads) : active_(true), queue_(active_) {
            for (int i = 0; i < num_threads; i++) {
                auto entry = std::mem_fn(&ThreadPool::ThreadWorkLoop);
                std::thread t(entry, this);
                thread_list.push_back(std::move(t));              
            }
        }   

        ~ThreadPool() {
            active_ = false;
            queue_.NotifyAll();
            // std::cout << "Waiting on threads to join...\n";
            for (int i = 0; i < thread_list.size(); i++) {
                thread_list[i].join();
            }
        }    

        void Schedule(std::function<void()> f) {
            queue_.Enqueue(f);
        }

    private:
        std::atomic_bool active_;

        void ThreadWorkLoop() {
            while (active_) {
                std::function<void()> f(queue_.Dequeue());
                if (f != NULL) f();
            } 
        }

        BlockingQueue<std::function<void()>> queue_;
        std::vector<std::thread> thread_list;
};