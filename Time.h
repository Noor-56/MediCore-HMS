#pragma once
#include<iostream>
#include<sstream>
using namespace std;
class Time {
    friend ostream& operator<<(ostream& out, const Time& obj);
    int hour, minute;
public:
    Time(int h = 0, int mn = 0);

    bool operator==(const Time& o) const;
    bool operator>(const Time& o)  const;
    string toString() const;
};