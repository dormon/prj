#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>


class ThreadPool{
  public:
    ThreadPool(int threads):shutdown_(false){
      // Create the specified number of threads
      threads_.reserve (threads);
      for (int i = 0; i < threads; ++i)
        threads_.emplace_back (std::bind (&ThreadPool::threadEntry, this, i));
    }
    ~ThreadPool (){
      {
        // Unblock any threads and tell them to stop
        std::unique_lock <std::mutex> l (lock_);

        shutdown_ = true;
        condVar_.notify_all();
      }
      // Wait for all threads to stop
      std::cerr << "Joining threads" << std::endl;
      for (auto& thread : threads_)
        thread.join();
    }
    void addJob(void(*fce)(ThreadPool*)){
      // Place a job on the queu and unblock a thread
      std::unique_lock <std::mutex> l (lock_);
      jobs_.emplace (fce);
      condVar_.notify_one();
    }
    void addJob(void(*fce)()){
      addJob([&](ThreadPool*){fce();});
    }
  protected:
    void threadEntry(int i){
      std::function<void(ThreadPool*)>job;
      while(1){
        {
          std::unique_lock <std::mutex> l (lock_);
          while (! shutdown_ && jobs_.empty()){
            std::cout << "waiting" << std::endl;

            condVar_.wait (l);
          }
          if (jobs_.empty ()){
            // No jobs to do and we are shutting down
            std::cerr << "Thread " << i << " terminates" << std::endl;
            return;
          }
          std::cerr << "Thread " << i << " does a job" << std::endl;
          job = std::move (jobs_.front ());
          jobs_.pop();
        }
        // Do the job without holding any locks
        job (this);
      }
    }
    std::mutex lock_;
    std::condition_variable condVar_;
    bool shutdown_;
    std::queue <std::function <void (ThreadPool*)>> jobs_;
    std::vector <std::thread> threads_;
};

void silly (int n)
{
  // A silly job for demonstration purposes
  std::cerr << "Sleeping for " << n << " seconds" << std::endl;
  std::this_thread::sleep_for (std::chrono::seconds (n));
}

void silly2(){
  std::cerr<<"silly2"<<std::endl;
  std::this_thread::sleep_for (std::chrono::seconds (2));
}

int main(){
  // Create two threads
  ThreadPool p (4);


  // Assign them 4 jobs
  p.addJob (std::bind (silly, 1));
  p.addJob (std::bind (silly, 2));
  p.addJob (std::bind (silly, 3));
  p.addJob (std::bind (silly, 4));
  p.addJob (std::bind (silly, 1));
  p.addJob (std::bind (silly, 1));
  p.addJob (std::bind (silly, 1));
  p.addJob (std::bind (silly, 2));
  p.addJob (std::bind (silly, 2));
  p.addJob (std::bind (silly, 2));
  p.addJob (silly2);


  return 0;
}





