#include"Date.h"
Date::Date(int dd, int mm, int yy) : d(dd), m(mm), y(yy) {}
int Date::getDay() const {
    return d;
}
int Date::getMonth() const {
    return m;
}
int Date::getYear() const {
    return y;
}
bool Date::operator==(const Date& o) const {
    return d == o.d && m == o.m && y == o.y;
}
bool Date::operator>(const Date& o) const {
    if (y != o.y)
        return y > o.y;
    if (m != o.m)
        return m > o.m;
    return d > o.d;
}
bool Date::operator<(const Date& o) const {
    return o > *this;
}
string Date::toString() const {
    string ds = (d < 10) ? "0" + to_string(d) : to_string(d);
    string ms = (m < 10) ? "0" + to_string(m) : to_string(m);
    return ds + "-" + ms + "-" + to_string(y);
}
ostream& operator<<(ostream& out, const Date& obj) {
    string ds = (obj.d < 10) ? "0" + to_string(obj.d) : to_string(obj.d);
    string ms = (obj.m < 10) ? "0" + to_string(obj.m) : to_string(obj.m);
    out << ds << "-" << ms << "-" << obj.y;
    return out;
}
