#include <string>
#include <vector>
using namespace std;

#define Line2(x) id##x

#define Line1(x) Line2(x)

#define UNIQ_ID Line1(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}