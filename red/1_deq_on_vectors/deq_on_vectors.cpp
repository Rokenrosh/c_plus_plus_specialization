#include <vector>
#include <stdexcept>
// #include "test_runner.h"

using namespace std;

template <typename T>
class Deque{
public:
    bool Empty() const;
    size_t Size() const;
    T& operator[](size_t index);
    const T& operator [](size_t index) const;
    T& At(size_t index);
    const T& At(size_t index) const;
    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;
    void PushFront(T x);
    void PushBack(T x);
private:
    vector<T> f_vector, b_vector;
};

template <typename T>
bool Deque<T>::Empty() const{
    return f_vector.empty() && b_vector.empty();
}

template<typename T>
size_t Deque<T>::Size() const{
    return f_vector.size() + b_vector.size();
}

template <typename T>
T& Deque<T>::operator[](size_t index){
    if(index < f_vector.size()){
        return f_vector[f_vector.size() - 1 - index];
    }
    else{
        return b_vector[index - f_vector.size()];
    }
}

template <typename T>
const T& Deque<T>::operator[](size_t index) const{
    if(index < f_vector.size()){
        return f_vector[f_vector.size() - 1 - index];
    }
    else{
        return b_vector[index - f_vector.size()];
    }
}

template <typename T>
T& Deque<T>::At(size_t index){
    if(index < Size()){
        if(index < f_vector.size()){
            return f_vector[f_vector.size() - 1 - index];
        }
        else{
            return b_vector[index - f_vector.size()];
        }
    }
    else{
        throw out_of_range("");
    }
}

template <typename T>
const T& Deque<T>::At(size_t index) const{
    if(index < Size()){
        if(index < f_vector.size()){
            return f_vector[f_vector.size() - 1 - index];
        }
        else{
            return b_vector[index - f_vector.size()];
        }
    }
    else{
        throw out_of_range("");
    }
}

template <typename T>
T& Deque<T>::Front(){
    if(!f_vector.empty()){
        return f_vector.back();
    }
    else{
        return b_vector.front();
    }
}

template <typename T>
const T& Deque<T>::Front() const{
    if(!f_vector.empty()){
        return f_vector.back();
    }
    else{
        return b_vector.front();
    }
}

template <typename T>
T& Deque<T>::Back(){
    if(!b_vector.empty()){
        return b_vector.back();
    }
    else{
        return f_vector.front();
    }
}

template <typename T>
const T& Deque<T>::Back() const{
    if(!b_vector.empty()){
        return b_vector.back();
    }
    else{
        return f_vector.front();
    }
}

template <typename T>
void Deque<T>::PushBack(T x){
    b_vector.push_back(x);
}

template <typename T>
void Deque<T>::PushFront(T x){
    f_vector.push_back(x);
}

// void TestDeque() {
//   Deque<int> t;
//   ASSERT(t.Empty());
//   t.PushBack(3);
//   t.PushBack(4);
//   t.PushBack(5);
//   t.PushFront(2);
//   t.PushFront(1);
//   t.PushFront(0);
//   ASSERT_EQUAL(t.Size(), 6);
//   ASSERT_EQUAL(t[4], 4);
//   ASSERT_EQUAL(t[0], 0);
//   ASSERT_EQUAL(t.Back(),5);
//   Deque<int> d;
//   d.PushFront(3);
//   ASSERT_EQUAL(d.Back(), 3);
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestDeque);
//   return 0;
// }
