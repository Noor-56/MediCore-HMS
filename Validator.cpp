#include"Validator.h"

bool Validator::isValidDate(const string& date) {
    if (date.length() != 10)
        return false;
    if (date[2] != '-' || date[5] != '-')
        return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5)
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }
    int d = stoi(date.substr(0, 2));    //String to Integer
    int mo = stoi(date.substr(3, 2));
    int y = stoi(date.substr(6, 4));
    time_t now = time(nullptr);
    struct tm t;
    localtime_s(&t, &now);
    if (y < t.tm_year + 1900)
        return false;  // tm_year stores how many years have passed since 1900.
    if (mo < 1 || mo > 12)
        return false;
    int dim[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };  //days in month
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
        dim[1] = 29;
    if (d < 1 || d > dim[mo - 1])
        return false;
    return true;
}
bool Validator::isValidSlot(const string& s) {
    const char* slots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
    for (int i = 0; i < 8; i++)
        if (s == slots[i])
            return true;
    return false;
}
bool Validator::isValidContact(const string& c) {
    if (c.length() != 11)
        return false;
    for (char ch : c)   //Range-based for loop
        if (ch < '0' || ch > '9')
            return false;
    return true;
}
bool Validator::isValidPassword(const string& p) {
    return p.length() >= 6;
}
bool Validator::isPositiveFloat(float v) {
    return v > 0.0f;
}
bool Validator::isMenuChoice(int c, int mn, int mx) {
    return c >= mn && c <= mx;
}
bool Validator::isValidID(int id) {
    return id > 0;
}
bool Validator::isValidAge(int age) {
    return age > 0 && age < 150;
}
bool Validator::isValidGender(char g) {
    return g == 'M' || g == 'm' || g == 'F' || g == 'f';
}
bool Validator::iequals(const string& a, const string& b) {
    if (a.length() != b.length()) return false;
    for (int i = 0; i < (int)a.length(); i++) {
        char ca = a[i], cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca += 32;
        if (cb >= 'A' && cb <= 'Z') cb += 32;
        if (ca != cb)
            return false;
    }
    return true;
}