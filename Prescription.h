#pragma once
#include<iostream>
#include"Date.h"
#include<sstream>
using namespace std;
class Prescription {
    friend ostream& operator<<(ostream& out, const Prescription& obj);
private:
    int prescriptionID, appointmentID, patientID, doctorID;
    Date date;
    string doctorName, medicines, notes;
public:
    Prescription(int pid = 0, int aid = 0, int patid = 0, int did = 0, int d = 0, int mo = 0, int y = 0, string dname = "", string meds = "", string nts = "");

    int getID() const;
    int getAppointmentID() const;
    int getPatientID() const;
    int getDoctorID() const;
    Date getDate() const;

    bool operator>(const Prescription& o) const;

    string toCSV() const;
};