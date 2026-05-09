#include"Doctor.h"
Doctor::Doctor(int id, string nm, string pw, string spec, float f, string cont) : Person(id, nm, pw), specialization(spec), fee(f), contact(cont) {
}
Doctor::~Doctor() {}
const string& Doctor::getSpec() const {
    return specialization;
}
const string& Doctor::getContact() const {
    return contact;
}
float Doctor::getFee() const {
    return fee;
}
void Doctor::printInfo() {
    cout << "ID: " << ID << "  Name: " << name << "  Spec: " << specialization << "  Fee: PKR " << fixed << setprecision(2) << fee << "  Contact: " << contact << "\n";
}
bool Doctor::login(int id, string pass) {
    return (id == ID && password == pass);
}
bool Doctor::operator==(const Doctor& o) const {
    return ID == o.ID;
}
bool Doctor::operator>(const Doctor& o) const {
    return ID > o.ID;
}
string Doctor::toCSV() const {
    ostringstream oss;
    oss << ID << "," << name << "," << specialization << "," << contact << "," << password << "," << fixed << setprecision(2) << fee;
    return oss.str();
}
ostream& operator<<(ostream& out, const Doctor& obj) {
    out << "[ Doctor: " << obj.name << " | Specialization: " << obj.specialization << " ]\n";
    return out;
}
