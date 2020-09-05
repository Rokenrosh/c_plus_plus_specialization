#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <functional>
#include <deque>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  size_t size() const {
    return pages.size();
  }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other){
    for(const auto& p : other.word_frequences){
      if(word_frequences.find(p.first) != word_frequences.end()){
        word_frequences[p.first] += p.second;
      }
      else{
        word_frequences[p.first] = p.second;
      }
    }
  }
};

template <typename Page>
Stats ExploreLine(const set<string>& key_words, Page line) {
  Stats result;
  for(auto& w: line){
    if(key_words.find(w) != key_words.end()){
      result.word_frequences[w]++;
    }
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  vector<future<Stats>> futuries;
  deque<string> text;
  string word;
  while(input){
    input >> word;
    text.push_back(move(word));
  }
  for(auto& page: Paginate(text, text.size() / 4)){
    futuries.push_back(async([key_words, page]{
      return ExploreLine(key_words, page);
    }));
  }
  Stats result;
  for(auto& f: futuries){
    result += f.get();
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
