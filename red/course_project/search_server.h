#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <mutex>
#include <future>
using namespace std;

template <typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(move(initial)){}

  struct Access {
    T& ref_to_value;
    lock_guard<mutex> lg;
  };

  Access GetAccess(){
    return Access{value, lock_guard(mtx)};
  }
private:
  T value;
  mutex mtx;
};

class InvertedIndex {
public:
  void Add(const string& document);
  const vector<pair<int, int>>& Lookup(const string& word) const;

  const string& GetDocument(int id) const {
    return docs[id];
  }

  int GetCount()const{
    return docs.size();
  }


private:
  vector<pair<int, int>> empty_vector = vector<pair<int, int>>();
  unordered_map<string, vector<pair<int, int>>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> futuries;
};