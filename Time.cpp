#include"Time.h"
Time::Time(int h, int mn) : hour(h), minute(mn) {}
bool Time::operator==(const Time& o) const {
    return hour == o.hour && minute == o.minute;
}
bool Time::operator>(const Time& o) const {
    if (hour != o.hour)
        return hour > o.hour;
    return minute > o.minute;
}
string Time::toString() const {
    string hs = (hour < 10) ? "0" + to_string(hour) : to_string(hour);
    string ms = (minute < 10) ? "0" + to_string(minute) : to_string(minute);
    return hs + ":" + ms;
}
ostream& operator<<(ostream& out, const Time& obj) {
    string hs = (obj.hour < 10) ? "0" + to_string(obj.hour) : to_string(obj.hour);
    string ms = (obj.minute < 10) ? "0" + to_string(obj.minute) : to_string(obj.minute);
    out << hs << ":" << ms;
    return out;
}