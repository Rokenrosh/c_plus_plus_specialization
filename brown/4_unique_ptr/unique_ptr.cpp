#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
  T* ptr_;
public:
  UniquePtr(){
    ptr_ =  nullptr_t();
  }
  UniquePtr(T * ptr): ptr_(ptr){}
  UniquePtr(const UniquePtr& other)= delete;
  UniquePtr(UniquePtr&& other){
    ptr_ = other.ptr_;
    other.ptr_ = nullptr_t();
  }
  UniquePtr& operator = (const UniquePtr&) = delete;
  UniquePtr& operator = (nullptr_t){
    Reset(nullptr_t());
    return *this;
  }
  UniquePtr& operator = (UniquePtr&& other){
    if(this != &other){
      Reset(other.ptr_);
      other.ptr_ = nullptr_t();
    }
    return *this;
  }
  ~UniquePtr(){
    delete ptr_;
  }

  T& operator * () const{
    return *ptr_;
  }

  T * operator -> () const{
    return ptr_;
  }

  T * Release(){
    T* new_ptr = ptr_;
    ptr_ = nullptr_t();
    return new_ptr;
  }

  void Reset(T * ptr){
    delete ptr_;
    ptr_ = ptr;
  }

  void Swap(UniquePtr& other){
    if(ptr_ == nullptr_t() && other.ptr_ == nullptr_t()){
      return;
    }
    else if(ptr_ == nullptr_t()){
      ptr_ = other.ptr_;
      other.ptr_ = nullptr_t();
    }
    else if(other.ptr_ == nullptr_t()){
      other.ptr_ = ptr_;
      ptr_ = nullptr_t();
    }
    else{
      T temp = *ptr_;
      *ptr_ = *other;
      *other = temp;
    }
  }

  T * Get() const{
    return ptr_;
  }
};


struct Item {
  static int counter;
  int value;
  Item(int v = 0): value(v) {
    ++counter;
  }
  Item(const Item& other): value(other.value) {
    ++counter;
  }
  ~Item() {
    --counter;
  }
};

int Item::counter = 0;


void TestLifetime() {
  Item::counter = 0;
  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    ptr.Reset(new Item);
    ASSERT_EQUAL(Item::counter, 1);
  }
  ASSERT_EQUAL(Item::counter, 0);

  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    auto rawPtr = ptr.Release();
    ASSERT_EQUAL(Item::counter, 1);

    delete rawPtr;
    ASSERT_EQUAL(Item::counter, 0);
  }
  ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
  UniquePtr<Item> ptr(new Item(42));
  ASSERT_EQUAL(ptr.Get()->value, 42);
  ASSERT_EQUAL((*ptr).value, 42);
  ASSERT_EQUAL(ptr->value, 42);
}

void Test(){
  UniquePtr<Item> ptr1(new Item(42));
  UniquePtr<Item> ptr2;
  ptr1.Swap(ptr2);
  ASSERT_EQUAL(ptr2.Get()->value, 42);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
  RUN_TEST(tr, Test);
}
