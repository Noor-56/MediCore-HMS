#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include"FileNotFoundException.h"
#include"Storage.h"
#include"Patient.h"
#include"Doctor.h"
#include"Admin.h"
#include"Bill.h"
#include"Prescription.h"
#include"Appointment.h"
using namespace std;
class FileHandler {
public:
    static string todayString();
    static void ensureFilesExist();
    static bool fileHasData(const string& filename);
    static void loadPatients(Storage<Patient>& s);
    static void loadDoctors(Storage<Doctor>& s);
    static void loadAdmins(Storage<Admin>& s);
    static void loadAppointments(Storage<Appointment>& s);
    static void loadBills(Storage<Bill>& s);
    static void loadPrescriptions(Storage<Prescription>& s);
    static void appendLine(const string& filename, const string& csv);
    static void rewritePatients(Storage<Patient>& s);
    static void rewriteDoctors(Storage<Doctor>& s);
    static void rewriteAppointments(Storage<Appointment>& s);
    static void rewriteBills(Storage<Bill>& s);
    static void rewritePrescriptions(Storage<Prescription>& s);
    static int maxPatientID(Storage<Patient>& s);
    static int maxDocID(Storage<Doctor>& s);
    static int maxApptID(Storage<Appointment>& s);
    static int maxBillID(Storage<Bill>& s);
    static int maxPresID(Storage<Prescription>& s);
    static void logSecurity(const string& role, int id, const string& result);
    static void archivePatient(Patient* p, Storage<Appointment>& apts, Storage<Prescription>& prs, Storage<Bill>& bls);
};
