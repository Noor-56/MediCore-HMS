#pragma once
#include<iostream>
#include <string>
using namespace std;
class Validator {
public:
    static bool isValidDate(const string& date);
    static bool isValidSlot(const string& s);
    static bool isValidContact(const string& c);
    static bool isValidPassword(const string& p);
    static bool isPositiveFloat(float v);
    static bool isMenuChoice(int c, int mn, int mx);
    static bool isValidID(int id);
    static bool isValidAge(int age);
    static bool isValidGender(char g);
    static bool iequals(const string& a, const string& b);
};