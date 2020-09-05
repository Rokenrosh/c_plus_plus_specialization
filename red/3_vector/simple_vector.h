#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector(): data(nullptr), end_(nullptr), capacity_(0){}  
  explicit SimpleVector(size_t size){
    data = new T[size];
    end_ = data + size;
    capacity_ = size;
  }
  ~SimpleVector(){
    delete[] data; 
  }

  T& operator[](size_t index){
    return data[index];
  }

  T* begin(){ return data; }
  T* end(){ return end_; }

  size_t Size() const{
    return end_ - data;
  }
  size_t Capacity() const{
    return capacity_;
  }
  void PushBack(const T& value){
    if(data == nullptr){
      data = new T[1];
      capacity_ = 1;
      end_ = data;
    }
    else if(Size() == Capacity()){
      T* temp = new T[Capacity() * 2];
      for(auto i = 0; i < Size(); ++i){
        temp[i] = data[i];
      }
      delete[] data;
      data = temp;
      end_ = data + Capacity();
      capacity_ *= 2;
    }
    *end_ = value;
    end_++;
  }

private:
  T* data;
  T* end_;
  size_t capacity_;
};
