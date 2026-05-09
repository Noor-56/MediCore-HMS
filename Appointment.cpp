#include"Appointment.h"
Appointment::Appointment(int id, int pid, int did, int d, int mo, int y, int h, int mi, string stat) : id(id), patientID(pid), doctorID(did), date(d, mo, y), time(h, mi), status(stat) {
}
int Appointment::getID() const {
    return id;
}
int Appointment::getPatientID() const {
    return patientID;
}
int Appointment::getDoctorID() const {
    return doctorID;
}
Date Appointment::getDate() const {
    return date;
}
Time Appointment::getTime() const {
    return time;
}
const string& Appointment::getStatus() const {
    return status;
}
void Appointment::setStatus(string s) {
    status = s;
}
bool Appointment::operator==(const Appointment& o) const {
    if (doctorID != o.doctorID)
        return false;
    if (!(date == o.date))
        return false;
    if (!(time == o.time))
        return false;
    if (status == "cancelled" || o.status == "cancelled")
        return false;
    return true;
}
bool Appointment::operator>(const Appointment& o) const {
    if (!(date == o.date))
        return date > o.date;
    return time > o.time;
}
string Appointment::toCSV() const {
    return to_string(id) + "," + to_string(patientID) + "," + to_string(doctorID) + "," + date.toString() + "," + time.toString() + "," + status;
}
ostream& operator<<(ostream& out, const Appointment& obj) {
    out << "Appt#" << obj.id << " | PatientID:" << obj.patientID << " | DoctorID:" << obj.doctorID << " | " << obj.date << " | " << obj.time << " | " << obj.status << "\n";
    return out;
}