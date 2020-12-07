#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;


bool IsSubdomain(string_view subdomain, string_view domain) {
  auto i = 0;
  auto j = 0;
  while (i < subdomain.size() && j < domain.size()) {
    if (subdomain[i++] != domain[j++]) {
      return false;
    }
  }
  bool result = ((subdomain.size() == domain.size()) || (i == subdomain.size() && j < domain.size() && domain[j] == '.'));
  return result;
}

// bool IsSubdomainReverse(string_view subdomain, string_view domain){
//   int i = 0;
//   int j = domain.size() - 1;
//   while (i < subdomain.size() && j >= 0) {
//     if (subdomain[i++] != domain[j--]) {
//       return false;
//     }
//   }
//   bool result = ((subdomain.size() == domain.size()) || (i == subdomain.size() && j >= 0 && domain[j] == '.'));
//   return result;
// }


vector<string> ReadDomains() {
  size_t count;
  cin >> count;

  vector<string> domains;
  for (size_t i = 0; i < count; ++i) {
    string domain;
    cin >> domain;
    domains.push_back(domain);
  }
  return domains;
}

void FilterDomains(vector<string>& banned_domains){
  size_t insert_pos = 0;
  for (string& domain : banned_domains) {
    if (insert_pos == 0 || !IsSubdomain(banned_domains[insert_pos - 1], domain)) {
      swap(banned_domains[insert_pos++], domain);
    }
  }
  banned_domains.resize(insert_pos);
}


int main() {
  vector<string> banned_domains = ReadDomains();
  vector<string> domains_to_check = ReadDomains();

  for (string& domain : banned_domains) {
    reverse(begin(domain), end(domain));
  }

  for (string& domain : domains_to_check) {
    reverse(begin(domain), end(domain));
  }
  sort(begin(banned_domains), end(banned_domains));

  FilterDomains(banned_domains);

  for (const string_view domain : domains_to_check) {
    if (const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
        it != begin(banned_domains) && !IsSubdomain(*prev(it), domain)) {
      cout << "Good" << endl;
    } else {
      if(it == begin(banned_domains) && !IsSubdomain(*it, domain)){
        cout << "Good" << endl;
      }else{
        cout << "Bad" << endl;
      }
    }
  }
  return 0;
}
