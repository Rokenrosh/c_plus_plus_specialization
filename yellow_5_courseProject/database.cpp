#include "database.h"
#include <algorithm>


void Database::Print(ostream& os) const{
    for (const auto& item : storage) {
      for (const string& event : item.second) {
        os << item.first << " " << event << endl;
      }
    }
}

void Database::Add(const Date& date, const string& event){
    if(set_storage[date].find(event) == end(set_storage[date])){
        set_storage[date].insert(event);
        storage[date].push_back(event);
    }
}

string Database::Last(const Date& date) const{
    auto it = storage.upper_bound(date);
    if(it == begin(storage)){
        throw invalid_argument("");
    }
    it--;
    stringstream ss;
    ss << it->first << " " << it->second.back();
    return ss.str();
}

