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

#include<typeinfo>
#include<cassert>

#include"Node.h"
#include"Viewport.h"
#include"Layer.h"
#include"Viewport2d.h"
#include"Node2d.h"

#include<typeinfo>
#include<typeindex>

int main(){
  auto root = new Viewport2d(glm::vec2(100.f,100.f));
  root->layers.push_back(new Layer());
  delete root;
  std::map<std::type_index,void*>asd;
  asd[typeid(int)]=nullptr;
  return 0;
}





