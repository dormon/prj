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

enum class Qualifier{};
class Type{};
class EmptyType: public Type{};
class UnqualifiedType: public Type{};
class QualifiedType: public Type{};
class AtomicType: public UnqualifiedType{};
class CompositeType: public UnqualifiedType{};
class AnyType: public AtomicType{};
class BitType: public AtomicType{};
class StructType: public CompositeType{};
class InfiniteType: public CompositeType{};
class FunctionType: public CompositeType{};

int main(){
  return 0;
}





