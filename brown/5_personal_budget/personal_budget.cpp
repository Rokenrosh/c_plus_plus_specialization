#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

namespace {
static const int SECONDS_IN_DAY = 60 * 60 * 24;
}

class Date{
public:
    Date(){}
    Date(int day, int month, int year): day_(day), month_(month), year_(year){}
    time_t AsTimestamp() const;
private:
    bool IsLeapYear(int year) const{
        if((year - 2000) % 4 == 0){
            return true;
        }
        else{
            return false;
        }
    }
    int day_;
    int month_;
    int year_;
};

class Budget{
public:
    double ComputeIncome(const Date& date_from, const Date& date_to);
    void PayTax(const Date& date_from, const Date& date_to, int percent);
    void Earn(const Date& date_from, const Date& date_to, int value);
    void Spend(const Date& date_from, const Date& date_to, int value);
private:
    unordered_map<time_t, double> incomes;
    unordered_map<time_t, double> spendings;
};

time_t Date::AsTimestamp() const {
  std::tm t;
  t.tm_sec   = 0;
  t.tm_min   = 0;
  t.tm_hour  = 0;
  t.tm_mday  = day_;
  t.tm_mon   = month_ - 1;
  t.tm_year  = year_ - 1900;
  t.tm_isdst = 0;
  return mktime(&t);
}

int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
  const time_t timestamp_to = date_to.AsTimestamp();
  const time_t timestamp_from = date_from.AsTimestamp();
  return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

void Budget::Earn(const Date& date_from, const Date& date_to, int value){
    int days = ComputeDaysDiff(date_to, date_from) + 1;
    double earn_per_day = double(value) / days;
    auto fromAsStamp = date_from.AsTimestamp();
    for(int i = 0; i < days; i++){
        incomes[fromAsStamp + i * SECONDS_IN_DAY] += earn_per_day;
    }
}

void Budget::PayTax(const Date& date_from, const Date& date_to, int percent){
    int days = ComputeDaysDiff(date_to, date_from) + 1;
    auto fromAsStamp = date_from.AsTimestamp();
    for(int i = 0; i < days; i++){
        incomes[fromAsStamp + i * SECONDS_IN_DAY] *= (1.0 - (percent / 100.0));
    }
}

double Budget::ComputeIncome(const Date& date_from, const Date& date_to){
    int days = ComputeDaysDiff(date_to, date_from) + 1;
    auto fromAsStamp = date_from.AsTimestamp();
    double result = 0;
    for(int i = 0; i < days; i++){
        result += incomes[fromAsStamp + i * SECONDS_IN_DAY] - spendings[fromAsStamp + i * SECONDS_IN_DAY];
    }
    return result;
}

void Budget::Spend(const Date& date_from, const Date& date_to, int value){
    int days = ComputeDaysDiff(date_to, date_from) + 1;
    auto fromAsStamp = date_from.AsTimestamp();
    double spend_per_day = static_cast<double>(value) / days;
    for(int i = 0; i < days; i++){
        spendings[fromAsStamp + i * SECONDS_IN_DAY] += spend_per_day;
    }
}

Date ParseDate(stringstream& ss){
    int day, month, year;
    if(ss){
        ss >> year;
    }
    ss.ignore(1);
    if(ss){
        ss >> month;
    }
    ss.ignore(1);
    if(ss){
        ss >> day;
    }
    return Date(day, month, year);
}

istream& operator >>(istream& is, Date& date){
    int day, month, year;
    if(is){
        is >> year;
    }
    is.ignore(1);
    if(is){
        is >> month;
    }
    is.ignore(1);
    if(is){
        is >> day;
    }
    date = Date(day, month, year);
    return is; 
}

void ParseQueries(istream& is = cin){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int q;
    is >> q;
    is.ignore();
    Budget budget;
    for(int i = 0; i < q; i++){
        string query;
        getline(is, query);
        //cerr << query << endl;
        stringstream ss(query);
        string command;
        ss >> command;
        Date from, to;
        ss >> from >> to;
        if(command == "ComputeIncome"){
            cout << setprecision(25) << budget.ComputeIncome(from, to) << endl;
        } else {
            int value;
            ss >> value;
            if(command == "PayTax"){
                budget.PayTax(from, to, value);
            } else if(command == "Earn"){
                budget.Earn(from, to, value);
            } else if(command == "Spend"){
                budget.Spend(from, to, value);
            }
        }
    }
}

int main(){
    ParseQueries(cin);
}