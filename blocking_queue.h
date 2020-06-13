#include <iostream>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

//Blocks on dequeue when empty
template <class T> class BlockingQueue {

    public:
        // BlockingQueue() {}
        BlockingQueue(std::atomic_bool &break_pred) : active_(break_pred) {}

        void Enqueue(T t) {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(t);
            lock.unlock();
            cond_var_.notify_one();
        } 

        T Dequeue() {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_var_.wait(lock, [this]() { return !this->queue_.empty() || !this->active_; });
            if(!active_ && queue_.empty()) {
                return NULL;
            }
            T t = queue_.front();
            queue_.pop();
            return t;
        }

        void NotifyAll() {
            cond_var_.notify_all();
        }

    private:
        std::atomic_bool &active_; //So the user can signal to stop blocking
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_var_;
};