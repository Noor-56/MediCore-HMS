#pragma once
#include<iostream>
#include <sstream>
#include <iomanip>
#include"Person.h"
using namespace std;
class Doctor : public Person {
    friend ostream& operator<<(ostream& out, const Doctor& obj);
private:
    string specialization;
    float fee;
    string contact;
public:
    Doctor(int id = 0, string nm = "", string pw = "", string spec = "", float f = 0.0f, string cont = "");
    ~Doctor();

    const string& getSpec() const;
    const string& getContact() const;
    float getFee() const;

    void printInfo() override;
    bool login(int id, string pass) override;

    bool operator==(const Doctor& o) const;
    bool operator>(const Doctor& o) const;

    string toCSV() const;
};