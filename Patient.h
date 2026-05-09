#pragma once
#include<iostream>
#include <sstream>
#include <iomanip>
#include"Person.h"
using namespace std;
class Patient : public Person {
    friend ostream& operator<<(ostream& out, const Patient& obj);
private:
    int age;
    char gender;
    float balance;
    string contact;
public:
    Patient(int id = 0, string nm = "", string pw = "", int ag = 0, char gen = 'M', float bal = 0.0f, string cont = "");
    ~Patient();

    float getbalance() const;
    void setbalance(float b);
    int getAge() const;
    char getGender() const;
    const string& getContact() const;

    void printInfo() override;
    bool login(int id, string pass) override;

    Patient& operator+=(float a);
    Patient& operator-=(float a);
    bool operator==(const Patient& o) const;
    bool operator>(const Patient& o)  const;

    string toCSV() const;
};