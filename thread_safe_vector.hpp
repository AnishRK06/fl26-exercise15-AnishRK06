#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <vector>

// Generic vector that's safe to share between threads.
// Same idea as the ThreadSafeQueue from lecture: one mutex guards the
// underlying std::vector, and a condition variable lets a popping thread
// sleep until something gets pushed. Pops come off the back (LIFO).
template <typename T>
class ThreadSafeVector {
public:
    ThreadSafeVector() = default;

    // a mutex can't be copied or moved, so this class can't be either
    ThreadSafeVector(const ThreadSafeVector&) = delete;
    ThreadSafeVector& operator=(const ThreadSafeVector&) = delete;

    // adds to the back. unlock before notify so the woken thread doesn't
    // wake up just to block on the mutex we're still holding
    void push_back(const T& value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            data_.push_back(value);
        }
        cond_.notify_one();
    }

    // blocks until the vector isn't empty, then pops the back into value.
    // the predicate handles spurious wakeups and another thread grabbing
    // the item first
    void wait_and_pop_back(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !data_.empty(); });
        value = data_.back();
        data_.pop_back();
    }

    // non-blocking version. returns false if there was nothing to pop
    bool try_pop_back(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (data_.empty()) return false;
        value = data_.back();
        data_.pop_back();
        return true;
    }

    // only a snapshot, another thread can change it right after we return
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.empty();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.size();
    }

private:
    std::vector<T> data_;
    mutable std::mutex mutex_;  // mutable so const reads can still lock
    std::condition_variable cond_;
};
