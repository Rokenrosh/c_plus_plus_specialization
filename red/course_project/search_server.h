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
  /*Метод UpdateDocumentBase заменяет текущую базу документов на новую, которая содержится в потоке document_input.
  При этом документ из первой строки этого потока будет иметь идентификатор (docid) 0, документ из второй строки — идентификатор 1 и т.д.
  Точно так же должен назначать идентификаторы документам и конструктор класса SearchServer.*/
  void UpdateDocumentBase(istream& document_input);
  /*Метод AddQueriesStream выполняет собственно поиск. Он принимает входной поток поисковых запросов и выходной поток для записи результатов поиска.
  Результатом обработки поискового запроса является набор из максимум пяти наиболее релевантных документов. В реальных поисковых системах метрика релевантности устроена довольно сложно.
  В рамках задачи в качестве метрики релевантности мы будем использовать суммарное количество вхождений всех слов запроса в документ. */
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> futuries;
};
