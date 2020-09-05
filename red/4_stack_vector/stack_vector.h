#pragma once

#include <stdexcept>
#include <array>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0){
    if(a_size > N){
      throw invalid_argument("size biiger than capacity");
    }
    size_ = a_size;
  }

  T& operator[](size_t index){
    return arr[index];
  }
  const T& operator[](size_t index) const{
    return arr.at(index);
  }

  auto begin(){
    return arr.begin();
  }
  auto end(){
    return arr.begin() + size_;
  }
  auto begin() const{
    return arr.begin();
  }
  auto end() const{
    return arr.begin() + size_;
  }

  size_t Size() const{
    return size_;
  }
  size_t Capacity() const{
    return N;
  }

  void PushBack(const T& value){
    if(Size() == Capacity()){
      throw overflow_error("can't push to full capacity");
    }
    else{
      arr[size_++] = value;
    }
  }
  T PopBack(){
    if(size_ == 0){
      throw underflow_error("can't pop from empty vector");
    }
    return arr[--size_];
  }

private:
  array<T, N> arr;
  size_t size_;
};

