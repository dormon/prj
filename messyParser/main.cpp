#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <iomanip>
#include <cmath>

using FCE = void*(*)(void*);

struct FceAndParam{
  FCE fce;
  void*param;
};

void*exec(void*ptr){
  auto f = (FceAndParam*)ptr;
  return f->fce(f->param);
}

int main()
{

  return 0;
}
