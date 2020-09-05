#include "stats.h"
#include <set>

set<string_view> valid_methods = {"GET", "POST", "PUT", "DELETE", "UNKNOWN"};
set<string_view> valid_urls = {"/", "/order", "/product", "/basket", "/help", "unknown"};

  Stats::Stats(){
    for(auto m: valid_methods){
      method_stats[m];
    }
    for(auto u: valid_urls){
      uri_stats[u];
    }
  }
  void Stats::AddMethod(string_view method){
    auto it = valid_methods.find(method);
    if(it == valid_methods.end()){
        method = "UNKNOWN";
    }
    else{
        method = *(it);
    }
    method_stats[method]++;
  }
  void Stats::AddUri(string_view uri){
    auto it = valid_urls.find(uri);
    if(it == valid_urls.end()){
        uri = "unknown";
    }
    else{
        uri = *(it);
    }
    uri_stats[uri]++;
  }
  const map<string_view, int>& Stats::GetMethodStats() const{
    return method_stats;
  }
  const map<string_view, int>& Stats::GetUriStats() const{
    return uri_stats;
  }

HttpRequest ParseRequest(string_view line){
  string_view method, uri, protocol;
  line.remove_prefix(min(line.find_first_not_of(" "), line.size()));
  size_t begin_pos = line.find_first_not_of(" ");
  size_t end_pos = line.find(' ');
  method = line.substr(begin_pos, end_pos);
  begin_pos = end_pos + 1;
  end_pos = line.find(' ', begin_pos);
  uri = line.substr(begin_pos, end_pos - begin_pos);
  protocol = line.substr(end_pos + 1);
  return {method, uri, protocol};
}