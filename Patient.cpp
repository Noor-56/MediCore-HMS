#include "Patient.h"
Patient::Patient(int id, string nm, string pw, int ag, char gen, float bal, string cont) : Person(id, nm, pw), age(ag), gender(gen), balance(bal), contact(cont) {
}
Patient::~Patient() {}
float Patient::getbalance() const {
    return balance;
}
void Patient::setbalance(float b) {
    balance = b;
}
int Patient::getAge() const {
    return age;
}
char Patient::getGender() const {
    return gender;
}
const string& Patient::getContact() const {
    return contact;
}
void Patient::printInfo() {
    cout << "Name: " << name << "  ID: " << ID
        << "  Age: " << age << "  Gender: " << gender
        << "  Contact: " << contact
        << "  Balance: PKR " << fixed << setprecision(2) << balance << "\n";
}
bool Patient::login(int id, string pass) {
    return (id == ID && password == pass);
}
Patient& Patient::operator+=(float a) {
    balance += a;
    return *this;
}
Patient& Patient::operator-=(float a) {
    balance -= a;
    return *this;
}
bool Patient::operator==(const Patient& o) const {
    return ID == o.ID;
}
bool Patient::operator>(const Patient& o) const {
    return ID > o.ID;
}
string Patient::toCSV() const {
    ostringstream oss;
    oss << ID << "," << name << "," << age << "," << gender << "," << contact << "," << password << "," << fixed << setprecision(2) << balance;
    return oss.str();
}
ostream& operator<<(ostream& out, const Patient& obj) {
    out << "[ Patient: " << obj.name << " | Balance: PKR " << fixed << setprecision(2) << obj.balance << " ]\n";
    return out;
}