#pragma once
#include<iostream>
#include<sstream>
#include"Date.h"
#include"Time.h"
using namespace std;
class Appointment {
    friend ostream& operator<<(ostream& out, const Appointment& obj);
private:
    int id, patientID, doctorID;
    Date date;
    Time time;
    string status;
public:
    Appointment(int id = 0, int pid = 0, int did = 0, int d = 0, int mo = 0, int y = 0, int h = 0, int mi = 0, string stat = "pending");

    int getID() const;
    int getPatientID() const;
    int getDoctorID() const;
    Date getDate() const;
    Time getTime() const;
    const string& getStatus() const;
    void setStatus(string s);

    bool operator==(const Appointment& o) const;
    bool operator>(const Appointment& o) const;

    string toCSV() const;
};