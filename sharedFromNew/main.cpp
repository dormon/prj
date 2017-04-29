#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class Texture{
  public:
    int a;
    float b;
    char*c;
    Texture(int a,float b){
      this->a = a;
      this->b = b;
      this->c = new char[this->a];
    }
    ~Texture(){
      delete[]this->c;
    }
};

void alloc(Texture**tex,float z){
  *tex = new Texture(int(z),z);
}

int main(){
  std::shared_ptr<Texture>tt;
  
  Texture*t;
  alloc(&t,13.3f);
  tt=std::shared_ptr<Texture>(t);
  return 0;
}
