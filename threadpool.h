#pragma once
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
        ThreadPool(size_t num_threads) : ThreadPool(num_threads, NULL) {}

        ThreadPool(size_t num_threads, std::function<void()> thread_init)
                   : active_(true),
                     active_threads_(0),
                     queue_(active_), 
                     thread_init_(thread_init) {
            for (int i = 0; i < num_threads; i++) {
                auto entry = std::mem_fn(&ThreadPool::ThreadWorkLoop);
                std::thread t(entry, this);
                thread_list_.push_back(std::move(t));              
            }
        }

        ~ThreadPool() {
            active_ = false;
            queue_.NotifyAll();
            for (int i = 0; i < thread_list_.size(); i++) {
                thread_list_[i].join();
            }
        }    

        void Schedule(std::function<void()> f) {
            queue_.Enqueue(f);
        }

        size_t NumThreads() {
            return thread_list_.size();
        }

        size_t NumInactiveThreads() {
            return NumThreads() - active_threads_;
        }

    private:
        std::function<void()> thread_init_; //Have each thread run a method before starting
        void ThreadWorkLoop() {
            if (thread_init_ != NULL) thread_init_();

            while (active_ || !queue_.Empty()) {
                std::function<void()> f(queue_.Dequeue());
                if (f != NULL) {
                    active_threads_++;
                    f();
                    active_threads_--;
                }
            } 
        }

        std::atomic_bool active_;
        BlockingQueue<std::function<void()>> queue_;
        std::vector<std::thread> thread_list_;
        std::atomic_size_t active_threads_;
};