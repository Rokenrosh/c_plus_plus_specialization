#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate(){
    if(!released.empty()){
      T* pointer = released.front();
      allocated.insert(pointer);
      released.pop();
      return pointer;
    }
    else{
      T* pointer = new T;
      allocated.insert(pointer);
      return pointer;
    }
  }
  T* TryAllocate(){
    if(released.empty()){
      return nullptr;
    }
    else{
      T* pointer = released.front();
      allocated.insert(pointer);
      released.pop();
      return pointer;
    }
  }

  void Deallocate(T* object){
    if(allocated.find(object) != allocated.end()){
      allocated.erase(object);
      released.push(object);
    }
    else{
      throw invalid_argument("");
    }
  }

  ~ObjectPool(){
    for(auto s : allocated){
      delete s;
    }
    allocated.clear();
    while(!released.empty()){
      delete released.front();
      released.pop();
    }
  }

private:
  queue<T*> released;
  set<T*> allocated;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
