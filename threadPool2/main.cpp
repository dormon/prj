#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

/*
class ThreadPool{
  public:
    ThreadPool(size_t nofThreads):shutdown(false){
      // Create the specified number of threads
      threads.reserve (nofThreads);
      for (size_t i = 0; i < nofThreads; ++i)
        threads.emplace_back (std::bind (&ThreadPool::threadEntry, this));
    }
    ~ThreadPool (){
      {
        // Unblock any threads and tell them to stop
        std::unique_lock <std::mutex> l (lock);

        shutdown = true;
        condVar.notify_all();
      }
      // Wait for all threads to stop
      std::cerr << "Joining threads" << std::endl;
      for (auto& thread : threads)
        thread.join();
    }
    void addJob(void(*fce)(ThreadPool*)){
      // Place a job on the queu and unblock a thread
      std::unique_lock <std::mutex> l (lock);
      jobs.emplace (fce);
      condVar.notify_one();
    }
    void addJob(void(*fce)()){
      std::unique_lock <std::mutex> l (lock);
      jobs.emplace ([&](ThreadPool*){fce();});
      condVar.notify_one();
    }
  protected:
    void threadEntry(){
      std::function<void(ThreadPool*)>job;
      while(1){
        {
          std::unique_lock <std::mutex> l (lock);
          while (! shutdown && jobs.empty()){
            condVar.wait (l);
          }
          if (jobs.empty ()){
            return;
          }
          job = std::move (jobs.front ());
          jobs.pop();
        }
        // Do the job without holding any locks
        job (this);
      }
    }
    std::mutex lock;
    std::condition_variable condVar;
    bool shutdown;
    std::queue <std::function <void (ThreadPool*)>> jobs;
    std::vector <std::thread> threads;
};

*/

class ThreadPool;

class ThreadWorker {
  private:
  ThreadPool *m_pool;

  public:
  ThreadWorker(ThreadPool *pool);
  void operator()();
};

class ThreadPool {
  public:
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&)      = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;

  ThreadPool(const int n_threads);
  void init();
  void shutdown();
  template <typename F, typename... Args>
  auto submit(F &&f, Args &&... args) -> std::future<decltype(f(args...))>;

  private:
  friend class ThreadWorker;
  std::vector<std::thread>          m_threads;
  bool                              m_shutdown;
  std::queue<std::function<void()>> m_queue;
  std::condition_variable           m_conditional_lock;
  std::mutex                        m_mutex;
  std::mutex queueAccessMutex;
};

ThreadPool::ThreadPool(const int n_threads)
    : m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {}

template <typename F, typename... Args>
auto ThreadPool::submit(F &&f, Args &&... args)
    -> std::future<decltype(f(args...))> {
  std::function<decltype(f(args...))()> func =
      std::bind(std::forward<F>(f), std::forward<Args>(args)...);
  auto task_ptr =
      std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

  std::function<void()> wrapper_func = [task_ptr]() { (*task_ptr)(); };

  std::unique_lock<std::mutex> lock(m_mutex);

  queueAccessMutex.lock();
  m_queue.push(wrapper_func);
  queueAccessMutex.unlock();

  m_conditional_lock.notify_one();


  return task_ptr->get_future();
}

ThreadWorker::ThreadWorker(ThreadPool *pool) : m_pool(pool) {}

void ThreadPool::init() {
  for (auto &t : m_threads) t = std::thread(ThreadWorker(this));
}

void ThreadPool::shutdown() {
  m_shutdown = true;
  queueAccessMutex.lock();
  m_conditional_lock.notify_all();

  for (auto &t : m_threads)
    if (t.joinable()) t.join();
}

void ThreadWorker::operator()() {
  std::function<void()> func;
  while (!m_pool->m_shutdown) {
    {
      m_pool->queueAccessMutex.lock();
      auto empty = m_pool->m_queue.empty();
      m_pool->queueAccessMutex.lock();

      if (empty) 
        m_pool->m_conditional_lock.wait(lock);

      if (m_pool->m_queue.empty()) continue;
      func = std::move(m_pool->m_queue.front());
      m_pool->m_queue.pop();
    }
    func();
  }
}

void silly(int n) {
  // A silly job for demonstration purposes
  std::cerr << "Sleeping for " << n << " seconds" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(n));
}

void silly2() {
  std::cerr << "silly2" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main() {
  // Create two threads
  ThreadPool p(4);

  return 0;
}
