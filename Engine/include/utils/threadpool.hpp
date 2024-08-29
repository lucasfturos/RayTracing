#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  private:
    std::atomic<bool> m_stop;
    mutable std::mutex m_mtx;
    std::condition_variable m_cv;
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;

  public:
    ThreadPool() : m_stop(false) {}

    void start(std::size_t numThreads =
                   std::max(1u, std::thread::hardware_concurrency())) {
        for (std::size_t i = 0; i < numThreads; ++i) {
            m_threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_mtx);
                        m_cv.wait(lock, [this] {
                            return m_stop.load(std::memory_order_relaxed) ||
                                   !m_tasks.empty();
                        });
                        if (m_stop.load(std::memory_order_relaxed) &&
                            m_tasks.empty()) {
                            break;
                        }
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_stop.store(true, std::memory_order_relaxed);
        }
        m_cv.notify_all();
        for (auto &thread : m_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        m_threads.clear();
    }

    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type> {
        using returnType = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            if (m_stop.load(std::memory_order_relaxed)) {
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            }
            m_tasks.emplace([task = std::move(task)]() { (*task)(); });
        }
        m_cv.notify_one();
        return res;
    }
};