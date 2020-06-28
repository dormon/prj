#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>
#include <cassert>

class Reactor{
  public:
    virtual~Reactor(){}
    virtual void react(
        std::vector<float>     &reaction,
        std::vector<float>const&input   ) = 0;
    virtual void reward(float r) = 0;
};

class Sum: public Reactor{
  public:
    Sum(std::vector<float>const&init):sum(init){}
    virtual void react(
        std::vector<float>     &reaction,
       std::vector<float>const&input   )override{
      assert(input.size() == sum.size());
      reaction = sum;
      std::transform(std::begin(sum),std::end(sum),std::cbegin(sum),std::cbegin(input),std::plus<float>());
    };
    virtual void reward(float r) override{
    }
  protected:
    std::vector<float>sum;
};

int main()
{
  return 0;
}
