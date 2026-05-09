#include"Person.h"
Person::Person(int ID, string n, string pw) : ID(ID), name(n), password(pw) {}
Person::~Person() {}
int Person::getID() const {
    return ID;
}
const string& Person::getName() const {
    return name;
}
const string& Person::getPassword() const {
    return password;
}