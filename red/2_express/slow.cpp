#include <algorithm>
#include <cmath>
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].insert(finish);
    reachable_lists_[finish].insert(start);
  }
  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {
        return result;
    }
    const set<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
      auto left_bound = reachable_stations.lower_bound(finish);
      auto right_bound = reachable_stations.upper_bound(finish);
      if(*left_bound == finish){
        return 0;
      }
      int result1 = INT_MAX, result2 = INT_MAX;
      if(left_bound != reachable_stations.begin()){
        result1 = abs(finish - *prev(left_bound));
      }
      if(right_bound != reachable_stations.end()){
        result2 = abs(finish - *right_bound);
      }
      result = min(result, min(result1, result2));
    }
    return result;
  }
private:
  map<int, set<int>> reachable_lists_;
};


int main() {
  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}
