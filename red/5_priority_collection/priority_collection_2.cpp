#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object){
    container.push_back({move(object), 0});
    priority.insert({0, current_id});
    return current_id++;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin)
    {
      for(auto it = range_begin; it != range_end; it++){
        *(ids_begin++) = Add(move(*it));
      }
    }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const{
    if(id > current_id || deleted_id.find(id) != deleted_id.end()){
      return false;
    }
    return true;
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const{
    return container[id].first;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id){
    container[id].second++;
    priority.erase({container[id].second - 1, id});
    priority.insert({container[id].second, id});
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const{
    auto last = priority.rbegin();
    return container[last->second];
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax(){
    auto last = *(priority.rbegin());
    deleted_id.insert(last.second);
    priority.erase(prev(priority.end()));
    return move(container[last.second]);
  }

private:
  Id current_id = 0;
  set<pair<int, Id>> priority;
  vector<pair<T, int>> container;
  set<Id> deleted_id;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");


  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto wt = strings.GetMax();
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

void TestRangeAdd(){
    PriorityCollection<StringNonCopyable> strings;

    std::vector<StringNonCopyable> words;
    words.emplace_back("a");
    words.emplace_back("b");
    words.emplace_back("c");

    std::vector<int> indexes;
    strings.Add(words.begin(), words.end(), std::back_inserter(indexes));
    for (int i : indexes)
        std::cout << i << ' ';
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, TestRangeAdd);
  return 0;
}
