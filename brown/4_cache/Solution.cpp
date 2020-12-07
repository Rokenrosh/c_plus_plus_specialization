#include "Common.h"
#include <unordered_map>
#include <set>
#include <mutex>
#include <algorithm>
#include <limits>
#include <iostream>
#include <list>

using namespace std;

// class LruCache : public ICache {
// public:
//   LruCache(
//       shared_ptr<IBooksUnpacker> books_unpacker,
//       const Settings& settings
//   ): settings_(settings), books_unpacker_(books_unpacker) { }

//   BookPtr GetBook(const string& book_name) override {
//     lock_guard<mutex> lg(m);

//     if(cache_.count(book_name)){
//       auto it = find_if(ranks_.begin(), ranks_.end(), [book_name](const BookRank& p){
//         return p.second->GetName() == book_name;
//       });
//       auto node = ranks_.extract(it);
//       node.value() = {numeric_limits<uint64_t>::max(), cache_[book_name]};
//       ranks_.insert(move(node));
//       return cache_[book_name];
//     }

//     BookPtr result = books_unpacker_->UnpackBook(book_name);

//     auto book_size = result->GetContent().size();

//     if(book_size > settings_.max_memory){
//       while(!cache_.empty()){
//         auto node = ranks_.extract(ranks_.begin());
//         cache_.erase(node.value().second->GetName());
//       }
//       memory_used = 0;
//       return result;
//     }

//     cache_[book_name] = result;
//     ranks_.insert({0, cache_[book_name]});

//     memory_used += cache_[book_name]->GetContent().size();

//     while(memory_used + book_size > settings_.max_memory && !cache_.empty()){
//       auto node = ranks_.extract(ranks_.begin());
//       memory_used -= node.value().second->GetContent().size();
//       cache_.erase(node.value().second->GetName());
//     }
//     return result;
//   }
// private:
//   using BookRank = pair<uint64_t, BookPtr>;

//   const Settings settings_;

//   shared_ptr<IBooksUnpacker> books_unpacker_;

//   unordered_map<string, BookPtr> cache_;
//   set<BookRank> ranks_;

//   size_t memory_used = 0;

//   mutex m;
// };


class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ): settings_(settings), books_unpacker_(books_unpacker) { }

  BookPtr GetBook(const string& book_name) override{
    lock_guard<mutex> lg(m);

    if(auto it = find_if(cache_.begin(), cache_.end(), 
    [book_name](const BookPtr& b){ return b->GetName() == book_name;});
    it != cache_.end()){
      cache_.push_back(move(*it));
      cache_.erase(it);
      return cache_.back();
    }

    BookPtr book = books_unpacker_->UnpackBook(book_name);

    auto book_size = book->GetContent().size();

    cache_.push_front(book);
    memory_used += book_size;

    while(memory_used + book_size > settings_.max_memory && !cache_.empty()){
      memory_used -= cache_.front()->GetContent().size();
      cache_.pop_front();
    }
    if(book_size > settings_.max_memory){
      return move(book);
    }

    return book;
  }

private:
  const Settings settings_;

  shared_ptr<IBooksUnpacker> books_unpacker_;

  list<BookPtr> cache_;

  size_t memory_used = 0;

  mutex m;
};

unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(books_unpacker, settings);
}
