#pragma once
#include<iostream>
#include<sstream>
using namespace std;
class Date {
    friend ostream& operator<<(ostream& out, const Date& obj);
    int d, m, y;
public:
    Date(int dd = 0, int mm = 0, int yy = 0);

    int getDay()   const;
    int getMonth() const;
    int getYear()  const;

    bool operator==(const Date& o) const;
    bool operator>(const Date& o)  const;
    bool operator<(const Date& o) const;

    string toString() const;
};