#include<timer_export.h>
#include<chrono>

class TIMER_EXPORT Timer{
  public:
    Timer();
    void reset();
    float getTimeFromLast();
    float getTimeFromStart();
  protected:
    std::chrono::time_point<std::chrono::high_resolution_clock>start;
    std::chrono::time_point<std::chrono::high_resolution_clock>last ;
};
