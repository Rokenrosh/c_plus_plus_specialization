#include "test_runner.h"
#include <vector>
#include <sstream>
#include <ostream>

void Test_1(){
    vector<Person> persons;
    ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 0);
}

void Test_2(){
  vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 40);
}

void Test_3(){
    std::stringstream ss;
    ss << "2\n31 1 0\n40 0 1";
    vector<Person> persons = ReadPersons(ss);
    Person p1 = {31, Gender::MALE, false};
    Person p2 = {40, Gender::FEMALE, true};
    ASSERT_EQUAL(persons[0], p1);
    ASSERT_EQUAL(persons[1], p2);
}

void Test_4(){
    std::stringstream ss;
    ss << "2\n31 1 1\n40 0 0";
    vector<Person> persons = ReadPersons(ss);
    ASSERT(persons[0].is_employed);
    ASSERT(!persons[1].is_employed);
}

void Test_5(){
    vector<Person> persons = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    AgeStats as = {40, 40, 55, 40, 80, 55, 78};
    ASSERT_EQUAL(ComputeStats(persons), as);
}

void Test_6(){
    vector<Person> persons = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    auto as = ComputeStats(persons);
    std::stringstream os;
    PrintStats(as, os);
    const std::string result = "Median age = 40\n"
    "Median age for females = 40\n"
    "Median age for males = 55\n"
    "Median age for employed females = 40\n"
    "Median age for unemployed females = 80\n"
    "Median age for employed males = 55\n"
    "Median age for unemployed males = 78\n";
    ASSERT_EQUAL(result, os.str());
}

int main(){
    TestRunner tr;
    tr.RunTest(Test_1, "Test_1");
    tr.RunTest(Test_2, "Test_2");
    tr.RunTest(Test_3, "Test_3");
    tr.RunTest(Test_4, "Test_4");
    tr.RunTest(Test_5, "Test_5");
    tr.RunTest(Test_6, "Test_6");
}