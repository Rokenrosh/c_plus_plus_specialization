#include <list>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(){
    int n;
    cin >> n;
    list<int> l;
    vector<list<int>::iterator> posisions(100'000 + 1, l.end());
    auto it = begin(l);
    for(int i = 0; i < n; ++i){
        int number, pos;
        cin >> number >> pos;
        posisions[number] = l.insert(posisions[pos], number);
    }
    for(const auto& el : l){
        cout << el << "\n";
    }
}