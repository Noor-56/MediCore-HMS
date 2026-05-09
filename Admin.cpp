#include"Admin.h"
Admin::Admin(int id, string nm, string pw) : Person(id, nm, pw) {}
Admin::~Admin() {}
void Admin::printInfo() {
    cout << "Admin | ID: " << ID << " | Name: " << name << "\n";
}
bool Admin::login(int id, string pass) {
    return (id == ID && password == pass);
}
string Admin::toCSV() const {
    return to_string(ID) + "," + name + "," + password;
}
ostream& operator<<(ostream& out, const Admin& /*obj*/) {
    out << "[ Admin Access Granted - MediCore ]\n"; return out;
}