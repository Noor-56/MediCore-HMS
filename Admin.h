#pragma once
#include<iostream>
#include"Person.h"
#include<sstream>
using namespace std;
class Admin : public Person {
    friend ostream& operator<<(ostream& out, const Admin& obj);
public:
    Admin(int id = 0, string nm = "", string pw = "");
    ~Admin();

    void printInfo() override;
    bool login(int id, string pass) override;

    string toCSV() const;
};