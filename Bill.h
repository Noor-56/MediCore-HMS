#pragma once
#include<iostream>
#include<sstream>
#include<iomanip>
#include"Date.h"
using namespace std;
class Bill {
    friend ostream& operator<<(ostream& out, const Bill& obj);
private:
    int billID, appointmentID, patientID;
    float amount;
    string status;
    Date date;
public:
    Bill(int id = 0, int aid = 0, int pid = 0, int d = 0, int mo = 0, int y = 0, float amt = 0.0f);

    int getID() const;
    int getAppointmentID() const;
    int getPatientID() const;
    float getAmount() const;
    const string& getStatus() const;
    Date getDate() const;
    void setStatus(string s);

    bool operator>(const Bill& o) const;

    string toCSV() const;
};