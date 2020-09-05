#include "test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Table
template <typename T>
class Table{
public:
  Table(size_t i, size_t j){
    array = vector<vector<T>>(i, vector<T>(j));
  }
  vector<T>& operator [](size_t index){
    return array[index];
  }
  const T& operator [](const size_t index) const{
    return array[index];
  }
  void Resize(size_t new_row, size_t new_col){
    array.resize(new_row);
    for(int i = 0; i < new_row; i++){
      array[i].resize(new_col);
    }
  }
  pair<size_t, size_t> Size() const{
    return make_pair(array.size(), array[0].size());
  }
  
private:
  vector<vector<T>> array; 
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
