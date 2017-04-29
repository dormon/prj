#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>

using namespace std;

int main(){
  map<int, int> m;
  m[1] = 2;
  m[3] = 6;
  m[88] = 888;
  for (const auto&[k, v] : m) {
    cout << k << "=  " << v << "\n";
  }



  return 0;
}
