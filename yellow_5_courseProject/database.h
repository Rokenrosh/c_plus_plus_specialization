#pragma once

#include "date.h"
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>

using namespace std;


class Database{
public:
    void Add(const Date& date, const string& event);
    void Print(ostream& os) const;

    template <class F>
    int RemoveIf(const F& f){
        int count = 0;
        for(auto it = begin(storage); it != end(storage); it++){
            auto del = stable_partition(begin(it->second), end(it->second),
            [it, f](string& event){
                return !f(it->first, event);
            });
            while(del != end(it->second)){
                set_storage[it->first].erase(it->second.back());
                it->second.pop_back();
                count++;
            }
        }
        for(auto it = begin(storage); it != end(storage);){
            if(it->second.empty()){
                it = storage.erase(it);
            }
            else {
                it++;
            }
        }
        return count;
    }

    template <class F>
    vector<string> FindIf(const F& f) const{
        vector<string> result;
        for(auto it = begin(storage); it != end(storage); it++){
            auto find_it = begin(it->second);
            while(find_it != end(it->second)){
                find_it = find_if(find_it , end(it->second),
                [it, f](const string& event){
                    return f(it->first, event);
                });
                if(find_it != end(it->second)){
                    stringstream ss;
                    ss << it->first << " " << *find_it;
                    result.push_back(ss.str());
                    find_it++;
                }
            }
        }
        return result;
    }

    string Last(const Date& date) const;
private:
    map<Date, vector<string>> storage;
    map<Date, set<string>> set_storage;
};