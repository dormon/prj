#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

typedef float(FCE)(float);

class Storer{
  public:
    std::vector<std::function<FCE>>fces;
    void add(FCE const&f){
      fces.push_back(f);
    }
    void add(std::function<FCE>const&f){
      fces.push_back(f);
    }
};

float times2(float x){return x*2;}

int main(){
  Storer st;
  st.add(times2);
  st.add([](float x){return x*3;});
  for(auto x:st.fces)
    std::cout<<x(32.3f)<<std::endl;
  return 0;
}
