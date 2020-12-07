void PrintStats(vector<Person> persons){
    auto gender = partition(persons.begin(), persons.end(),[](const Person& p){
        return p.gender == Gender::MALE;
    });

    auto employed_male = partition(persons.begin(), gender, [](const Person& p){
        return p.is_employed;
    });

    auto employed_female = partition(gender, persons.end(), [](const Person& p){
        return p.is_employed;
    });

    cout << "Median age = " << ComputeMedianAge(persons.begin(), persons.end()) << '\n';
    cout << "Median age for females = " << ComputeMedianAge(gender, persons.end()) << '\n';
    cout << "Median age for males = " << ComputeMedianAge(persons.begin(), gender) << '\n';
    cout << "Median age for employed females = " << ComputeMedianAge(gender, employed_female) << '\n';
    cout << "Median age for unemployed females = " << ComputeMedianAge(employed_female, persons.end()) << '\n';
    cout << "Median age for employed males = " << ComputeMedianAge(persons.begin(), employed_male) << '\n';
    cout << "Median age for unemployed males = " << ComputeMedianAge(employed_male, gender);
}