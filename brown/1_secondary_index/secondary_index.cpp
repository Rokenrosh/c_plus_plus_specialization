#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};


struct Iters{
  multimap<int, Record*>::iterator timestamp_iter;
  multimap<int, Record*>::iterator karma_iter;
  multimap<string, Record*>::iterator user_iter;
};


// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record){
    auto ins = storage.emplace(record.id, record);
    if(ins.second){
      auto it_t = storage_by_time.emplace(record.timestamp, &ins.first->second);
      auto it_k = storage_by_karma.emplace(record.karma, &ins.first->second);
      auto it_u = storage_by_user.emplace(record.user, &ins.first->second);
      secondary_indexes.insert({record.id, {it_t, it_k, it_u}});
    }
    return ins.second;
  }
  const Record* GetById(const string& id) const{
    auto it = storage.find(id);
    if(it != storage.end()){
      return &it->second;
    }
    else{
      return nullptr;
    }
  }
  bool Erase(const string& id){
    size_t result = storage.erase(id);
    if(result > 0){
      auto it = secondary_indexes.find(id);
      storage_by_time.erase(it->second.timestamp_iter);
      storage_by_karma.erase(it->second.karma_iter);
      storage_by_user.erase(it->second.user_iter);
    }
    return result;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const{
    auto first = storage_by_time.lower_bound(low);
    auto last = storage_by_time.upper_bound(high);
    for(; first != last; first++){
      if(!callback(*first->second)){
        break;
      }
    }
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const{
    auto first = storage_by_karma.lower_bound(low);
    auto last = storage_by_karma.upper_bound(high);
    for(; first != last; first++){
      if(!callback(*first->second)){
        break;
      }
    }
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const{
    auto first = storage_by_user.lower_bound(user);
    auto last = storage_by_user.upper_bound(user);
    for(; first != last; first++){
      if(!callback(*first->second)){
        break;
      }
    }
  }
private:
  unordered_map<string, Record> storage;
  multimap<int, Record*> storage_by_time;
  multimap<int, Record*> storage_by_karma;
  multimap<string, Record*> storage_by_user;
  multimap<string, Iters> secondary_indexes;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
