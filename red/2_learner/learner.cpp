#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    int currNum = dict.size();
    dict.insert(begin(words), end(words));
    return dict.size() - currNum;
  }

  vector<string> KnownWords() {
    return {begin(dict), end(dict)};
  }
};

int main() {
  Learner learner;
  string line;
  while (getline(cin, line)) {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    cout << learner.Learn(words) << "\n";
  }
  cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    cout << word << "\n";
  }
}
