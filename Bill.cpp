#include"Bill.h"
Bill::Bill(int id, int aid, int pid, int d, int mo, int y, float amt) : billID(id), appointmentID(aid), patientID(pid), amount(amt), status("unpaid"), date(d, mo, y) {
}
int Bill::getID() const {
    return billID;
}
int Bill::getAppointmentID() const {
    return appointmentID;
}
int Bill::getPatientID() const {
    return patientID;
}
float Bill::getAmount() const {
    return amount;
}
const string& Bill::getStatus() const {
    return status;
}
Date Bill::getDate() const {
    return date;
}
void Bill::setStatus(string s) {
    status = s;
}
bool Bill::operator>(const Bill& o) const {
    return billID > o.billID;
}
string Bill::toCSV() const {
    ostringstream oss;
    oss << billID << "," << patientID << "," << appointmentID << "," << fixed << setprecision(2) << amount << "," << status << "," << date.toString();
    return oss.str();
}
ostream& operator<<(ostream& out, const Bill& obj) {
    out << "Bill#" << obj.billID << " | Appt#" << obj.appointmentID << " | Amount: PKR " << fixed << setprecision(2) << obj.amount << " | Status: " << obj.status << " | Date: " << obj.date << "\n";
    return out;
}
