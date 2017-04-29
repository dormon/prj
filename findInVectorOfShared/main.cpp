#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class D: public std::enable_shared_from_this<D>{
  public:
    int data;
    D(int a){
      this->data=a;
    }
    std::shared_ptr<D> me(){
      return this->shared_from_this();
    }
};

int main(){
  std::vector<std::shared_ptr<D>>v;
  v.push_back(std::make_shared<D>(23));
  v.push_back(std::make_shared<D>(13));
  v.push_back(std::make_shared<D>(3));
  v.push_back(std::make_shared<D>(2));
  if(v[2]==v[2]->me())std::cout<<"v[2]==v[2]->me()"<<std::endl;

  std::find(v.begin(),v.end(),v[2]->me());
  return 0;
}


