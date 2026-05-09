#pragma once
#include<iostream>
using namespace std;
class Person {
protected:
    int ID;
    string name;
    string password;
public:
    Person(int ID = 0, string n = "", string pw = "");
    virtual ~Person();

    int getID() const;
    const string& getName() const;
    const string& getPassword() const;

    virtual void printInfo() = 0;
    virtual bool login(int id, string pass) = 0;
};