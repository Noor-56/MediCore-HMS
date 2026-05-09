#include"Prescription.h"
Prescription::Prescription(int pid, int aid, int patid, int did, int d, int mo, int y, string dname, string meds, string nts) : prescriptionID(pid), appointmentID(aid), patientID(patid), doctorID(did), date(d, mo, y), doctorName(dname), medicines(meds), notes(nts) {
}
int Prescription::getID() const {
    return prescriptionID;
}
int Prescription::getAppointmentID() const {
    return appointmentID;
}
int Prescription::getPatientID() const {
    return patientID;
}
int Prescription::getDoctorID() const {
    return doctorID;
}
Date Prescription::getDate() const {
    return date;
}
bool Prescription::operator>(const Prescription& o) const {
    return !(date > o.date) && !(date == o.date);
}
string Prescription::toCSV() const {
    return to_string(prescriptionID) + "," + to_string(appointmentID) + "," + to_string(patientID) + "," + to_string(doctorID) + "," + date.toString() + "," + medicines + "," + notes;
}
ostream& operator<<(ostream& out, const Prescription& obj) {
    out << "Prescription#" << obj.prescriptionID << " | Dr." << obj.doctorName << " | Date: " << obj.date << "\n  Medicines: " << obj.medicines << "\n  Notes: " << obj.notes << "\n";
    return out;
}
