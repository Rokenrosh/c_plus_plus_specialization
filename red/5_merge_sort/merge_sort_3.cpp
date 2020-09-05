#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end){
    int size = range_end - range_begin;
    if(size < 2){
        return;
    }
    vector<typename RandomIt::value_type> elements;
    move(range_begin, range_end, back_inserter(elements));
    auto it1 = begin(elements) + size / 3 ;
    auto it2 = it1 + size / 3;
    MergeSort(begin(elements), it1);
    MergeSort(it1, it2);
    MergeSort(it2, end(elements));
    vector<typename RandomIt::value_type> temp;
    merge(make_move_iterator(begin(elements)), make_move_iterator(it1), make_move_iterator(it1), make_move_iterator(it2), back_inserter(temp));
    merge(make_move_iterator(begin(temp)), make_move_iterator(end(temp)), make_move_iterator(it2), make_move_iterator(end(elements)), range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
