#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define LOCALTIME(tm_ptr, time_ptr) localtime_s(tm_ptr, time_ptr)
#else
#define LOCALTIME(tm_ptr, time_ptr) localtime_r(time_ptr, tm_ptr)
#endif

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

#include"Admin.h"
#include"Appointment.h"
#include"Bill.h"
#include"Date.h"
#include"Doctor.h"
#include"FileHandler.h"
#include"FileNotFoundException.h"
#include"HospitalException.h"
#include"InsufficientFundsException.h"
#include"InvalidInputException.h"
#include"Patient.h"
#include"Person.h"
#include"Prescription.h"
#include"Time.h"
#include"Validator.h"
#include"SlotUnavailableException.h"
#include"Storage.h"

using namespace std;
static string inputWord(const string& prompt) {
    string val;
    cout << prompt;
    cin >> val;
    return val;
}

static string inputLine(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    return val;
}

void registerPatient(Storage<Patient>& pats) {
    cout << "\n--- New Patient Registration ---\n";
    cin.ignore();

    string name = inputLine("Full Name: ");
    while (name.empty()) {
        cout << "Name cannot be empty.\n";
        name = inputLine("Full Name: ");
    }

    int age = 0;
    while (true) {
        cout << "Age: "; cin >> age;
        if (Validator::isValidAge(age))
            break;
        cout << "Invalid age. Enter a value between 1 and 149.\n";
    }

    char gender = ' ';
    while (true) {
        cout << "Gender (M/F): ";
        cin >> gender;
        if (Validator::isValidGender(gender)) {
            gender = (gender == 'm' || gender == 'M') ? 'M' : 'F';
            break;
        }
        cout << "Enter M or F.\n";
    }

    string contact;
    cin.ignore();
    while (true) {
        contact = inputLine("Contact (11 digits): ");
        if (Validator::isValidContact(contact))
            break;
        cout << "Invalid contact. Must be exactly 11 digits.\n";
    }

    string password;
    while (true) {
        password = inputLine("Password (min 6 chars): ");
        if (Validator::isValidPassword(password))
            break;
        cout << "Password too short.\n";
    }

    float balance = 0.0f;
    while (true) {
        cout << "Initial Balance (PKR, enter 0 for none): ";
        cin >> balance;
        if (balance >= 0.0f)
            break;
        cout << "Balance cannot be negative.\n";
    }

    int newID = FileHandler::maxPatientID(pats) + 1;
    Patient* p = new Patient(newID, name, password, age, gender, balance, contact);
    pats.add(p);
    FileHandler::appendLine("patients.txt", p->toCSV());
    cout << "Registration successful! Your Patient ID is: " << newID << "\nPlease note your ID — you will need it to log in.\n";
}

void setupFirstAdmin(Storage<Admin>& admins) {
    cout << "\n================================================\n" << " MediCore First-Time Setup\n" << " No admin account found. Please create one now.\n" << "================================================\n";

    string name;
    while (true) {
        cout << "Enter Admin Full Name: ";
        getline(cin, name);
        if (!name.empty())
            break;
        cout << "Name cannot be empty.\n";
    }

    string password;
    while (true) {
        cout << "Enter Admin Password (minimum 6 characters): ";
        getline(cin, password);
        if (Validator::isValidPassword(password)) break;
        cout << "Password too short. Please use at least 6 characters.\n";
    }

    Admin* a = new Admin(1, name, password);
    admins.add(a);
    FileHandler::appendLine("admin.txt", a->toCSV());
    cout << "\nAdmin account created successfully!\n" << "  Admin Name : " << name << "\n" << "  Admin ID   : 1  (use this ID to log in)\n" << "  Password   : [as entered]\n";
}

void setupInitialDoctors(Storage<Doctor>& docs) {
    cout << "\n================================================\n" << " No doctors found in the system.\n" << " Please add at least one doctor to continue.\n" << "================================================\n";

    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        cout << "\n--- Add New Doctor ---\n";

        string name;
        while (true) {
            cout << "Doctor Full Name (e.g. Dr. Sara Khan): ";
            getline(cin, name);
            if (!name.empty())
                break;
            cout << "Name cannot be empty.\n";
        }

        string spec;
        while (true) {
            cout << "Specialization (e.g. Cardiology): ";
            getline(cin, spec);
            if (!spec.empty())
                break;
            cout << "Specialization cannot be empty.\n";
        }

        string cont;
        while (true) {
            cout << "Contact Number (exactly 11 digits): ";
            getline(cin, cont);
            if (Validator::isValidContact(cont))
                break;
            cout << "Invalid contact. Must be exactly 11 numeric digits.\n";
        }

        string pass;
        while (true) {
            cout << "Password (minimum 6 characters): ";
            getline(cin, pass);
            if (Validator::isValidPassword(pass))
                break;
            cout << "Password too short.\n";
        }

        float fee = 0.0f;
        while (true) {
            cout << "Consultation Fee (PKR): ";
            if (cin >> fee && fee > 0.0f) {
                cin.ignore(1000, '\n');
                break;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Fee must be a positive amount.\n";
        }

        int newID = FileHandler::maxDocID(docs) + 1;
        Doctor* d = new Doctor(newID, name, pass, spec, fee, cont);
        docs.add(d);
        FileHandler::appendLine("doctors.txt", d->toCSV());

        cout << "\nDoctor added successfully!\n" << "  Name           : " << name << "\n" << "  Specialization : " << spec << "\n" << "  Doctor ID      : " << newID << "  (use this ID to log in)\n";

        cout << "\nAdd another doctor? (y/n): ";
        cin >> addMore;
        cin.ignore(1000, '\n');  
    }
    cout << "Doctor setup complete. You can add more doctors from the Admin Menu.\n";
}

void bookAppointment(Patient* pat, Storage<Doctor>& docs, Storage<Appointment>& apts, Storage<Bill>& bills) {
    string spec;
    cin.ignore();
    cout << "Enter specialization to search: ";
    getline(cin, spec);

    bool found = false;
    for (int i = 0; i < docs.getCount(); i++) {
        if (Validator::iequals(docs.get(i)->getSpec(), spec)) {
            cout << "  ID:" << docs.get(i)->getID() << "  Name:" << docs.get(i)->getName() << "  Fee:PKR " << docs.get(i)->getFee() << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No doctors available for that specialization.\n";
        return;
    }
  
    cout << "Enter Doctor ID: ";
    int dID;
    cin >> dID;
    Doctor* doc = docs.getByID(dID);
    if (!doc) {
        cout << "Doctor not found.\n";
        return;
    }
    if (!Validator::iequals(doc->getSpec(), spec)) {
        cout << "Doctor not found.\n";
        return;
    }
    
    if (pat->getbalance() < doc->getFee()) {
        try {
            throw InsufficientFundsException();
        }
        catch (InsufficientFundsException& e) {
            cout << e.what() << "\n";
            return;
        }
    }
   
    string dateStr;
    int dateAttempts = 0;
    while (true) {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> dateStr;
        if (Validator::isValidDate(dateStr))
            break;
        cout << "Invalid date. Use format DD-MM-YYYY (year must be current or future).\n";
        if (++dateAttempts == 3) {
            cout << "Too many attempts.\n";
            return;
        }
    }
    int dd = stoi(dateStr.substr(0, 2));
    int dm = stoi(dateStr.substr(3, 2));
    int dy = stoi(dateStr.substr(6, 4));
    const char* allSlots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
    auto showAvail = [&]() {
        cout << "Available slots: ";
        for (int s = 0; s < 8; s++) {
            bool taken = false;
            for (int i = 0; i < apts.getCount(); i++) {
                Appointment* a = apts.get(i);
                if (a->getDoctorID() == dID && a->getDate().toString() == dateStr && a->getTime().toString() == allSlots[s] && a->getStatus() != "cancelled") {
                    taken = true;
                    break;
                }
            }
            if (!taken)
                cout << allSlots[s] << " ";
        }
        cout << "\n";
        };
    showAvail();

    string slot;
    while (true) {
        cout << "Enter time slot: ";
        cin >> slot;
        if (!Validator::isValidSlot(slot)) {
            cout << "Invalid slot.\n";
            continue;
        }
        bool taken = false;
        for (int i = 0; i < apts.getCount(); i++) {
            Appointment* a = apts.get(i);
            if (a->getDoctorID() == dID && a->getDate().toString() == dateStr && a->getTime().toString() == slot && a->getStatus() != "cancelled") {
                taken = true;
                break;
            }
        }
        if (taken) {
            try {
                throw SlotUnavailableException();
            }
            catch (SlotUnavailableException& e) {
                cout << e.what() << "\n";
                showAvail();
                continue;
            }
        }
        break;
    }
    int newAID = FileHandler::maxApptID(apts) + 1;
    int newBID = FileHandler::maxBillID(bills) + 1;
    int sh = stoi(slot.substr(0, 2)), sm = stoi(slot.substr(3, 2));
    string today = FileHandler::todayString();
    int td = stoi(today.substr(0, 2));
    int tm = stoi(today.substr(3, 2));
    int ty = stoi(today.substr(6, 4));

    Appointment* na = new Appointment(newAID, pat->getID(), dID, dd, dm, dy, sh, sm, "pending");
    apts.add(na);
    FileHandler::appendLine("appointments.txt", na->toCSV());

    Bill* nb = new Bill(newBID, newAID, pat->getID(), td, tm, ty, doc->getFee());
    bills.add(nb);
    FileHandler::appendLine("bills.txt", nb->toCSV());

    cout << "Appointment booked successfully. Appointment ID: " << newAID << "\n";
    cout << "Bill of PKR " << fixed << setprecision(2) << doc->getFee() << " created. Go to 'Pay Bill' to pay it.\n";
}


void cancelAppointment(Patient* pat, Storage<Doctor>& docs, Storage<Appointment>& apts, Storage<Bill>& bills) {

    bool any = false;
    for (int i = 0; i < apts.getCount(); i++) {
        Appointment* a = apts.get(i);
        if (a->getPatientID() == pat->getID() && a->getStatus() == "pending") {
            Doctor* d = docs.getByID(a->getDoctorID());
            cout << "  Appt#" << a->getID() << " | Dr." << (d ? d->getName() : "?") << " | " << a->getDate() << " | " << a->getTime() << "\n";
            any = true;
        }
    }
    if (!any) {
        cout << "You have no pending appointments.\n";
        return;
    }

    cout << "Enter Appointment ID to cancel: ";
    int aID;
    cin >> aID;
    Appointment* appt = apts.getByID(aID);
    if (!appt || appt->getPatientID() != pat->getID() || appt->getStatus() != "pending") {
        cout << "Invalid appointment ID.\n";
        return;
    }
    appt->setStatus("cancelled");
    FileHandler::rewriteAppointments(apts);
    for (int i = 0; i < bills.getCount(); i++) {
        if (bills.get(i)->getAppointmentID() == aID) {
            bills.get(i)->setStatus("cancelled");
            break;
        }
    }
    FileHandler::rewriteBills(bills);

    cout << "Appointment cancelled. Associated bill has been cancelled.\n";
}
void viewMyAppointments(Patient* pat, Storage<Doctor>& docs, Storage<Appointment>& apts) {
    Storage<Appointment> tmp(200);
    for (int i = 0; i < apts.getCount(); i++)
        if (apts.get(i)->getPatientID() == pat->getID())
            tmp.add(new Appointment(*apts.get(i)));
    if (tmp.getCount() == 0) {
        cout << "No appointments found.\n";
        return;
    }
    tmp.sortAsc();
    cout << left << setw(8) << "ID" << setw(16) << "Doctor" << setw(16) << "Spec" << setw(14) << "Date" << setw(8) << "Slot" << "Status\n" << string(78, '-') << "\n";
    for (int i = 0; i < tmp.getCount(); i++) {
        Appointment* a = tmp.get(i);
        Doctor* d = docs.getByID(a->getDoctorID());
        cout << left << setw(8) << a->getID() << setw(16) << (d ? d->getName() : "?") << setw(16) << (d ? d->getSpec() : "?") << setw(14) << a->getDate().toString() << setw(8) << a->getTime().toString() << a->getStatus() << "\n";
    }
}

void viewMedicalRecords(Patient* pat, Storage<Doctor>& docs, Storage<Prescription>& prs) {
    Storage<Prescription> tmp(200);
    for (int i = 0; i < prs.getCount(); i++)
        if (prs.get(i)->getPatientID() == pat->getID())
            tmp.add(new Prescription(*prs.get(i)));
    if (tmp.getCount() == 0) {
        cout << "No medical records found.\n";
        return;
    }
    tmp.sortDesc();
    for (int i = 0; i < tmp.getCount(); i++) {
        Prescription* p = tmp.get(i);
        Doctor* d = docs.getByID(p->getDoctorID());
        cout << "Date: " << p->getDate().toString() << "  Doctor: " << (d ? d->getName() : "?") << "\n";
        cout << *p;
    }
}

void viewMyBills(Patient* pat, Storage<Bill>& bills) {
    float total = 0.0f;
    bool any = false;
    cout << left << setw(8) << "BillID" << setw(10) << "ApptID" << setw(14) << "Amount(PKR)" << setw(12) << "Status" << "Date\n" << string(58, '-') << "\n";
    for (int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if (b->getPatientID() != pat->getID())
            continue;
        cout << left << setw(8) << b->getID() << setw(10) << b->getAppointmentID() << setw(14) << fixed << setprecision(2) << b->getAmount() << setw(12) << b->getStatus() << b->getDate().toString() << "\n";
        if (b->getStatus() == "unpaid") total += b->getAmount();
        any = true;
    }
    if (!any) {
        cout << "No bills found.\n";
        return;
    }
    cout << "Total outstanding unpaid: PKR " << fixed << setprecision(2) << total << "\n";
}

void payBill(Patient* pat, Storage<Bill>& bills) {
    bool any = false;
    for (int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if (b->getPatientID() == pat->getID() && b->getStatus() == "unpaid") {
            cout << "  Bill#" << b->getID() << " | PKR " << b->getAmount() << " | " << b->getDate().toString() << "\n";
            any = true;
        }
    }
    if (!any) {
        cout << "No unpaid bills.\n";
        return;
    }

    int bID;
    cout << "Enter Bill ID to pay: ";
    cin >> bID;
    Bill* bill = bills.getByID(bID);
    if (!bill || bill->getPatientID() != pat->getID() || bill->getStatus() != "unpaid") {
        cout << "Invalid Bill ID.\n"; return;
    }
    if (pat->getbalance() < bill->getAmount()) {
        try {
            throw InsufficientFundsException();
        }
        catch (InsufficientFundsException& e) {
            cout << e.what() << "\n";
            return;
        }
    }
    *pat -= bill->getAmount();
    bill->setStatus("paid");
    FileHandler::rewriteBills(bills);

    Storage<Patient> tmp;
    FileHandler::loadPatients(tmp);
    Patient* fp = tmp.getByID(pat->getID());
    if (fp)
        fp->setbalance(pat->getbalance());
    FileHandler::rewritePatients(tmp);

    cout << "Bill paid successfully. Remaining balance: PKR " << fixed << setprecision(2) << pat->getbalance() << "\n";
}

void topUp(Patient* pat) {
    int attempts = 0;
    while (attempts < 3) {
        float amt;
        cout << "Enter amount to add (PKR): ";
        cin >> amt;
        try {
            if (!Validator::isPositiveFloat(amt))
                throw InvalidInputException();
            *pat += amt;
            Storage<Patient> tmp;
            FileHandler::loadPatients(tmp);
            Patient* fp = tmp.getByID(pat->getID());
            if (fp)
                fp->setbalance(pat->getbalance());
            FileHandler::rewritePatients(tmp);
            cout << "Balance updated. New balance: PKR " << fixed << setprecision(2) << pat->getbalance() << "\n";
            return;
        }
        catch (InvalidInputException& e) {
            cout << e.what() << "\n";
            attempts++;
        }
    }
    cout << "Too many invalid attempts.\n";
}

void patientMenu(Patient* pat, Storage<Doctor>& docs, Storage<Appointment>& apts, Storage<Bill>& bills, Storage<Prescription>& prs) {
    while (true) {
        cout << "\nWelcome, " << pat->getName() << "\nBalance: PKR " << fixed << setprecision(2) << pat->getbalance() << "\n========================\n" << "1. Book Appointment\n2. Cancel Appointment\n" << "3. View My Appointments\n4. View My Medical Records\n" << "5. View My Bills\n6. Pay Bill\n7. Top Up Balance\n8. Logout\n" << "Choice: ";
        int c;
        cin >> c;
        if (c == 1)
            bookAppointment(pat, docs, apts, bills);
        else if (c == 2)
            cancelAppointment(pat, docs, apts, bills);
        else if (c == 3)
            viewMyAppointments(pat, docs, apts);
        else if (c == 4)
            viewMedicalRecords(pat, docs, prs);
        else if (c == 5)
            viewMyBills(pat, bills);
        else if (c == 6)
            payBill(pat, bills);
        else if (c == 7)
            topUp(pat);
        else if (c == 8) {
            cout << "Logged out.\n";
            break;
        }
        else
            cout << "Invalid choice.\n";
    }
}

void viewTodayAppts(Doctor* doc, Storage<Patient>& pats, Storage<Appointment>& apts) {
    string today = FileHandler::todayString();
    Storage<Appointment> tmp(200);
    for (int i = 0; i < apts.getCount(); i++) {
        Appointment* a = apts.get(i);
        if (a->getDoctorID() == doc->getID() && a->getDate().toString() == today)
            tmp.add(new Appointment(*a));
    }
    if (tmp.getCount() == 0) {
        cout << "No appointments scheduled for today.\n";
        return;
    }
    tmp.sortAsc();
    cout << left << setw(8) << "ApptID" << setw(22) << "Patient" << setw(8) << "Slot" << "Status\n" << string(50, '-') << "\n";
    for (int i = 0; i < tmp.getCount(); i++) {
        Appointment* a = tmp.get(i);
        Patient* p = pats.getByID(a->getPatientID());
        cout << left << setw(8) << a->getID() << setw(22) << (p ? p->getName() : "?") << setw(8) << a->getTime().toString() << a->getStatus() << "\n";
    }
}

void markStatus(Doctor* doc, Storage<Appointment>& apts, Storage<Bill>& bills, const string& newStatus) {
    string today = FileHandler::todayString();
    bool any = false;
    for (int i = 0; i < apts.getCount(); i++) {
        Appointment* a = apts.get(i);
        if (a->getDoctorID() == doc->getID() && a->getDate().toString() == today && a->getStatus() == "pending") {
            cout << "  Appt#" << a->getID() << " | " << a->getTime() << "\n";
            any = true;
        }
    }
    if (!any) {
        cout << "No pending appointments today.\n";
        return;
    }

    int aID;
    cout << "Enter Appointment ID: ";
    cin >> aID;
    Appointment* appt = apts.getByID(aID);
    if (!appt || appt->getDoctorID() != doc->getID() || appt->getStatus() != "pending" || appt->getDate().toString() != today) {
        cout << "Invalid Appointment ID.\n"; return;
    }
    appt->setStatus(newStatus);
    FileHandler::rewriteAppointments(apts);
    if (newStatus == "no-show") {
        for (int i = 0; i < bills.getCount(); i++)
            if (bills.get(i)->getAppointmentID() == aID) {
                bills.get(i)->setStatus("cancelled");
                break;
            }
        FileHandler::rewriteBills(bills);
        cout << "Appointment marked as no-show.\n";
    }
    else {
        cout << "Appointment marked as completed.\n";
    }
}

void writePrescription(Doctor* doc,
    Storage<Appointment>& apts,
    Storage<Prescription>& prs) {
    int aID;
    cout << "Enter Appointment ID: ";
    cin >> aID;
    Appointment* appt = apts.getByID(aID);
    if (!appt || appt->getDoctorID() != doc->getID() || appt->getStatus() != "completed") {
        cout << "Invalid appointment (must be yours and completed).\n";
        return;
    }
    for (int i = 0; i < prs.getCount(); i++)
        if (prs.get(i)->getAppointmentID() == aID) {
            cout << "Prescription already written for this appointment.\n";
            return;
        }

    cin.ignore();
    char meds[500], notes[300];
    cout << "Medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    cin.getline(meds, 500);
    cout << "Notes (max 300 chars): ";
    cin.getline(notes, 300);

    string today = FileHandler::todayString();
    int d = stoi(today.substr(0, 2)), mo = stoi(today.substr(3, 2)), y = stoi(today.substr(6, 4));
    int newID = FileHandler::maxPresID(prs) + 1;

    Prescription* pr = new Prescription(newID, aID, appt->getPatientID(), doc->getID(), d, mo, y, doc->getName(), string(meds), string(notes));
    prs.add(pr);
    FileHandler::appendLine("prescriptions.txt", pr->toCSV());
    cout << "Prescription saved.\n";
}

void viewPatientHistory(Doctor* doc, Storage<Patient>& pats, Storage<Appointment>& apts, Storage<Prescription>& prs) {
    int pid;
    cout << "Enter Patient ID: ";
    cin >> pid;
    Patient* pat = pats.getByID(pid);
    bool ok = false;
    if (pat) {
        for (int i = 0; i < apts.getCount(); i++) {
            Appointment* a = apts.get(i);
            if (a->getPatientID() == pid && a->getDoctorID() == doc->getID() && a->getStatus() == "completed") {
                ok = true;
                break;
            }
        }
    }
    if (!ok) {
        cout << "Access denied. You can only view records of your own patients.\n";
        return;
    }
    Storage<Prescription> tmp(200);
    for (int i = 0; i < prs.getCount(); i++) {
        Prescription* p = prs.get(i);
        if (p->getPatientID() == pid && p->getDoctorID() == doc->getID())
            tmp.add(new Prescription(*p));
    }
    if (tmp.getCount() == 0) {
        cout << "No prescriptions found.\n";
        return;
    }
    tmp.sortDesc();
    for (int i = 0; i < tmp.getCount(); i++)
        cout << *tmp.get(i);
}

void doctorMenu(Doctor* doc, Storage<Patient>& pats, Storage<Appointment>& apts, Storage<Bill>& bills, Storage<Prescription>& prs) {
    while (true) {
        cout << "\nWelcome, " << doc->getName() << " | Specialization: " << doc->getSpec() << "\n===============================================\n" << "1. View Today's Appointments\n2. Mark Appointment Complete\n" << "3. Mark Appointment No-Show\n4. Write Prescription\n" << "5. View Patient Medical History\n6. Logout\n" << "Choice: ";
        int c;
        cin >> c;
        if (c == 1)
            viewTodayAppts(doc, pats, apts);
        else if (c == 2)
            markStatus(doc, apts, bills, "completed");
        else if (c == 3)
            markStatus(doc, apts, bills, "no-show");
        else if (c == 4)
            writePrescription(doc, apts, prs);
        else if (c == 5)
            viewPatientHistory(doc, pats, apts, prs);
        else if (c == 6) {
            cout << "Logged out.\n";
            break;
        }
        else
            cout << "Invalid choice.\n";
    }
}

void addDoctor(Storage<Doctor>& docs) {
    cin.ignore();
    string name = inputLine("Name: ");
    while (name.empty()) {
        cout << "Name cannot be empty.\n";
        name = inputLine("Name: ");
    }

    string spec = inputLine("Specialization: ");
    while (spec.empty()) {
        cout << "Specialization cannot be empty.\n";
        spec = inputLine("Specialization: ");
    }

    string cont;
    while (true) {
        cont = inputLine("Contact (11 digits): ");
        if (Validator::isValidContact(cont))
            break;
        cout << "Invalid contact.\n";
    }

    string pass;
    while (true) {
        pass = inputLine("Password (min 6 chars): ");
        if (Validator::isValidPassword(pass))
            break;
        cout << "Password too short.\n";
    }

    float fee = 0.0f;
    while (true) {
        cout << "Fee (PKR): ";
        cin >> fee;
        if (Validator::isPositiveFloat(fee))
            break;
        cout << "Fee must be positive.\n";
    }

    int newID = FileHandler::maxDocID(docs) + 1;
    Doctor* d = new Doctor(newID, name, pass, spec, fee, cont);
    docs.add(d);
    FileHandler::appendLine("doctors.txt", d->toCSV());
    cout << "Doctor added successfully. ID: " << newID << "\n";
}

void removeDoctor(Storage<Doctor>& docs, Storage<Appointment>& apts) {
    docs.printAll();
    int dID; cout << "Enter Doctor ID to remove: ";
    cin >> dID;
    for (int i = 0; i < apts.getCount(); i++)
        if (apts.get(i)->getDoctorID() == dID && apts.get(i)->getStatus() == "pending") {
            cout << "Cannot remove doctor with pending appointments.\n"; return;
        }
    if (!docs.remove(dID)) {
        cout << "Doctor not found.\n";
        return;
    }
    FileHandler::rewriteDoctors(docs);
    cout << "Doctor removed.\n";
}

void viewAllPatients(Storage<Patient>& pats, Storage<Bill>& bills) {
    cout << left << setw(6) << "ID" << setw(20) << "Name" << setw(5) << "Age" << setw(8) << "Gender" << setw(14) << "Contact" << setw(12) << "Balance" << "Unpaid Bills\n" << string(80, '-') << "\n";
    for (int i = 0; i < pats.getCount(); i++) {
        Patient* p = pats.get(i);
        int unpaid = 0;
        for (int j = 0; j < bills.getCount(); j++)
            if (bills.get(j)->getPatientID() == p->getID() && bills.get(j)->getStatus() == "unpaid")
                unpaid++;
        cout << left << setw(6) << p->getID() << setw(20) << p->getName() << setw(5) << p->getAge() << setw(8) << p->getGender() << setw(14) << p->getContact() << setw(12) << fixed << setprecision(2) << p->getbalance() << unpaid << "\n";
    }
}

void viewUnpaidBills(Storage<Patient>& pats, Storage<Bill>& bills) {
    string today = FileHandler::todayString();
    time_t todayT;
    {
        struct tm t = {};
        t.tm_mday = stoi(today.substr(0, 2));
        t.tm_mon = stoi(today.substr(3, 2)) - 1;
        t.tm_year = stoi(today.substr(6, 4)) - 1900;
        t.tm_isdst = -1;
        todayT = mktime(&t);
    }
    cout << left << setw(8) << "BillID" << setw(12) << "Patient" << setw(12) << "Amount" << "Date\n" << string(52, '-') << "\n";
    int found = 0;
    for (int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if (b->getStatus() != "unpaid")

            continue;
        found++;
        Patient* p = pats.getByID(b->getPatientID());
        Date bd = b->getDate();
        string dcol = bd.toString();
        struct tm bt = {};
        bt.tm_mday = bd.getDay();
        bt.tm_mon = bd.getMonth() - 1;
        bt.tm_year = bd.getYear() - 1900;
        bt.tm_isdst = -1;
        time_t billT = mktime(&bt);
        if (difftime(todayT, billT) > 7 * 24 * 3600.0)
            dcol += " [OVERDUE]";
        cout << left << setw(8) << b->getID() << setw(12) << (p ? p->getName() : "?") << setw(12) << fixed << setprecision(2) << b->getAmount() << dcol << "\n";
    }
    if (found == 0)
        cout << "No unpaid bills.\n";
}
void dischargePatient(Storage<Patient>& pats, Storage<Appointment>& apts, Storage<Bill>& bills, Storage<Prescription>& prs) {
    int pid;
    cout << "Enter Patient ID: ";
    cin >> pid;
    Patient* p = pats.getByID(pid);
    if (!p) {
        cout << "Patient not found.\n";
        return;
    }
    for (int i = 0; i < bills.getCount(); i++)
        if (bills.get(i)->getPatientID() == pid && bills.get(i)->getStatus() == "unpaid") {
            cout << "Cannot discharge patient with unpaid bills.\n";
            return;
        }
    for (int i = 0; i < apts.getCount(); i++)
        if (apts.get(i)->getPatientID() == pid && apts.get(i)->getStatus() == "pending") {
            cout << "Cannot discharge patient with pending appointments.\n";
            return;
        }
    FileHandler::archivePatient(p, apts, prs, bills);
    pats.remove(pid);
    FileHandler::rewritePatients(pats);
    for (int i = apts.getCount() - 1; i >= 0; i--)
        if (apts.get(i)->getPatientID() == pid)
            apts.remove(apts.get(i)->getID());
    FileHandler::rewriteAppointments(apts);
    for (int i = bills.getCount() - 1; i >= 0; i--)
        if (bills.get(i)->getPatientID() == pid)
            bills.remove(bills.get(i)->getID());
    FileHandler::rewriteBills(bills);
    for (int i = prs.getCount() - 1; i >= 0; i--)
        if (prs.get(i)->getPatientID() == pid)
            prs.remove(prs.get(i)->getID());
    FileHandler::rewritePrescriptions(prs);
    cout << "Patient discharged and archived successfully.\n";
}

void viewSecurityLog() {
    ifstream f("security_log.txt");
    if (!f.is_open()) {
        cout << "No security events logged.\n";
        return;
    }
    string line;
    bool any = false;
    while (getline(f, line))
        if (!line.empty()) {
            cout << line << "\n";
            any = true;
        }
    if (!any)
        cout << "No security events logged.\n";
}

void dailyReport(Storage<Patient>& pats, Storage<Doctor>& docs, Storage<Appointment>& apts, Storage<Bill>& bills) {
    string today = FileHandler::todayString();
    int pending = 0, completed = 0, noshow = 0, cancelled = 0;
    for (int i = 0; i < apts.getCount(); i++) {
        Appointment* a = apts.get(i);
        if (a->getDate().toString() != today)
            continue;
        if (a->getStatus() == "pending")
            pending++;
        else if (a->getStatus() == "completed")
            completed++;
        else if (a->getStatus() == "no-show")
            noshow++;
        else if (a->getStatus() == "cancelled")
            cancelled++;
    }
    float revenue = 0.0f;
    for (int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if (b->getDate().toString() == today && b->getStatus() == "paid")
            revenue += b->getAmount();
    }
    cout << "\n--- Daily Report: " << today << " ---\n" << "Total appointments today: " << (pending + completed + noshow + cancelled) << " (Pending:" << pending << " Completed:" << completed << " No-show:" << noshow << " Cancelled:" << cancelled << ")\n";
    if ((pending + completed + noshow + cancelled) == 0)
        cout << "No appointments scheduled for today.\n";
    cout << "Revenue collected today: PKR " << fixed << setprecision(2) << revenue << "\n" << "\nPatients with outstanding unpaid bills:\n";
    bool anyUnpaid = false;
    for (int i = 0; i < pats.getCount(); i++) {
        Patient* p = pats.get(i);
        float owed = 0.0f;
        for (int j = 0; j < bills.getCount(); j++)
            if (bills.get(j)->getPatientID() == p->getID() && bills.get(j)->getStatus() == "unpaid")
                owed += bills.get(j)->getAmount();
        if (owed > 0.0f) {
            cout << "  " << left << setw(22) << p->getName() << "PKR " << fixed << setprecision(2) << owed << "\n";
            anyUnpaid = true;
        }
    }
    if (!anyUnpaid) cout << "None\n";
    if ((pending + completed + noshow + cancelled) > 0) {
        cout << "\nDoctor-wise summary:\n";
        for (int i = 0; i < docs.getCount(); i++) {
            Doctor* d = docs.get(i);
            int dc = 0, dp = 0, dn = 0;
            for (int j = 0; j < apts.getCount(); j++) {
                Appointment* a = apts.get(j);
                if (a->getDoctorID() != d->getID() || a->getDate().toString() != today) continue;
                if (a->getStatus() == "completed") dc++;
                else if (a->getStatus() == "pending")   dp++;
                else if (a->getStatus() == "no-show")   dn++;
            }
            if (dc + dp + dn > 0)
                cout << "  " << left << setw(22) << d->getName()
                << " Completed:" << dc << " Pending:" << dp << " No-show:" << dn << "\n";
        }
    }
}

void adminMenu(Admin*, Storage<Doctor>& docs, Storage<Patient>& pats, Storage<Appointment>& apts, Storage<Bill>& bills, Storage<Prescription>& prs) {
    while (true) {
        cout << "\nAdmin Panel - MediCore\n======================\n" << "1. Add Doctor\n2. Remove Doctor\n3. View All Patients\n" << "4. View All Doctors\n5. View All Appointments\n" << "6. View Unpaid Bills\n7. Discharge Patient\n" << "8. View Security Log\n9. Generate Daily Report\n10. Logout\n" << "Choice: ";
        int c;
        cin >> c;
        if (c == 1)
            addDoctor(docs);
        else if (c == 2)
            removeDoctor(docs, apts);
        else if (c == 3)
            viewAllPatients(pats, bills);
        else if (c == 4)
            docs.printAll();
        else if (c == 5) {
            Storage<Appointment> tmp(500);
            for (int i = 0; i < apts.getCount(); i++)
                tmp.add(new Appointment(*apts.get(i)));
            tmp.sortDesc();
            if (tmp.getCount() == 0)
                cout << "No appointments found.\n";
            for (int i = 0; i < tmp.getCount(); i++)
                cout << *tmp.get(i);
        }
        else if (c == 6)
            viewUnpaidBills(pats, bills);
        else if (c == 7)
            dischargePatient(pats, apts, bills, prs);
        else if (c == 8)
            viewSecurityLog();
        else if (c == 9)
            dailyReport(pats, docs, apts, bills);
        else if (c == 10) {
            cout << "Logged out.\n";
            break;
        }
        else
            cout << "Invalid choice.\n";
    }
}
void loadAllData(Storage<Patient>& pats, Storage<Doctor>& docs, Storage<Admin>& admins, Storage<Appointment>& apts, Storage<Bill>& bills, Storage<Prescription>& prs) {
    try {
        FileHandler::loadPatients(pats);
    }
    catch (FileNotFoundException& e) {
        cerr << e.what() << "\n";
    }
    try {
        FileHandler::loadDoctors(docs);
    }
    catch (FileNotFoundException& e) {
        cerr << e.what() << "\n";
    }
    try {
        FileHandler::loadAdmins(admins);
    }
    catch (FileNotFoundException& e) {
        cerr << e.what() << "\n";
    }
    FileHandler::loadAppointments(apts);
    FileHandler::loadBills(bills);
    FileHandler::loadPrescriptions(prs);
}

sf::RenderWindow statusWin;
sf::Font sfFont;

void updateStatusBar(const string& role = "", const string& user = "", float bal = -1.0f) {
    if (!statusWin.isOpen())
        return;
    sf::Event e;
    while (statusWin.pollEvent(e))
        if (e.type == sf::Event::Closed) statusWin.close();

    statusWin.clear(sf::Color(30, 30, 45));

    sf::RectangleShape bar({ 520, 4 });
    bar.setFillColor(sf::Color(56, 189, 248));
    statusWin.draw(bar);

    sf::Text title("MediCore HMS", sfFont, 15);
    title.setFillColor(sf::Color(56, 189, 248));
    title.setStyle(sf::Text::Bold);
    title.setPosition(12, 10);
    statusWin.draw(title);

    if (role.empty()) {
        sf::Text idle("No user logged in", sfFont, 13);
        idle.setFillColor(sf::Color(100, 116, 139));
        idle.setPosition(12, 34);
        statusWin.draw(idle);
    }
    else {
        
        sf::RectangleShape badge({ (float)(role.size() * 8 + 16), 20 });
        badge.setPosition(12, 32);
        sf::Color bc = (role == "Admin") ? sf::Color(239, 68, 68)
            : (role == "Doctor") ? sf::Color(34, 197, 94)
            : sf::Color(99, 102, 241);
        badge.setFillColor(bc);
        statusWin.draw(badge);

        sf::Text roleT(role, sfFont, 12);
        roleT.setFillColor(sf::Color::White);
        roleT.setPosition(16, 34);
        statusWin.draw(roleT);

        sf::Text nameT(user, sfFont, 13);
        nameT.setFillColor(sf::Color(241, 245, 249));
        nameT.setStyle(sf::Text::Bold);
        nameT.setPosition(12, 58);
        statusWin.draw(nameT);

    }
    sf::RectangleShape bot({ 520, 1 });
    bot.setPosition(0, 99);
    bot.setFillColor(sf::Color(55, 65, 81));
    statusWin.draw(bot);

    statusWin.display();
}

void showSplash() {

    if (!sfFont.loadFromFile("C:/Windows/Fonts/segoeui.ttf"))
        sfFont.loadFromFile("C:/Windows/Fonts/arial.ttf");

    sf::RenderWindow splash(sf::VideoMode(500, 300), "MediCore",
        sf::Style::Titlebar | sf::Style::Close);
    splash.setFramerateLimit(60);
    sf::Clock clk;

    while (splash.isOpen()) {
        sf::Event e;
        while (splash.pollEvent(e)) {
            if (e.type == sf::Event::Closed) splash.close();
            if (e.type == sf::Event::KeyPressed &&
                e.key.code == sf::Keyboard::Enter) splash.close();
        }

        float t = clk.getElapsedTime().asSeconds();

        splash.clear(sf::Color(30, 30, 45));

        sf::RectangleShape topBar({ 500, 5 });
        topBar.setFillColor(sf::Color(56, 189, 248));
        splash.draw(topBar);

        sf::Text title("MediCore", sfFont, 52);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color(56, 189, 248));
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin(tb.width / 2, tb.height / 2);
        title.setPosition(250, 110);
        splash.draw(title);

        sf::Text sub("Hospital Management System", sfFont, 17);
        sub.setFillColor(sf::Color(148, 163, 184));
        sf::FloatRect sb = sub.getLocalBounds();
        sub.setOrigin(sb.width / 2, sb.height / 2);
        sub.setPosition(250, 162);
        splash.draw(sub);

        sf::Uint8 alpha = (sf::Uint8)(128 + 127 * sin(t * 3.0f));
        sf::Text prompt("Press ENTER to continue", sfFont, 13);
        prompt.setFillColor(sf::Color(100, 116, 139, alpha));
        sf::FloatRect pb = prompt.getLocalBounds();
        prompt.setOrigin(pb.width / 2, pb.height / 2);
        prompt.setPosition(250, 220);
        splash.draw(prompt);

        splash.display();
    }

    statusWin.create(sf::VideoMode(520, 100), "MediCore - Status",
        sf::Style::Titlebar);
    statusWin.setPosition({ 1380, 10 });   
    updateStatusBar();                    
}

void runLoginLoop(Storage<Patient>& pats, Storage<Doctor>& docs, Storage<Admin>& admins, Storage<Appointment>& apts, Storage<Bill>& bills, Storage<Prescription>& prs) {
    while (true) {
        cout << "\nWelcome to MediCore Hospital Management System\n" << "===============================================\n" << "1. Login as Patient\n2. Login as Doctor\n3. Login as Admin\n" << "4. Register as New Patient\n5. Exit\n" << "Choice: ";
        int role;
        cin >> role;

        if (role == 5)
            break;

        if (role == 4) {
            registerPatient(pats);
            continue;
        }

        if (!Validator::isMenuChoice(role, 1, 3)) {
            cout << "Invalid choice.\n";
            continue;
        }
        string roleLabel = (role == 1) ? "Patient" : (role == 2) ? "Doctor" : "Admin";
        cout << "\n--- " << roleLabel << " Login ---\n";

        int attempts = 0;
        bool auth = false;
        while (attempts < 3 && !auth) {
            int id = 0;
            string pass;
            cout << "Enter " << roleLabel << " ID: ";
            cin >> id;
            cout << "Enter " << roleLabel << " Password: ";
            cin >> pass;

            if (role == 1) {
                Patient* p = pats.getByID(id);
                if (p && p->login(id, pass)) {
                    auth = true;
                    updateStatusBar("Patient", p->getName(), p->getbalance());
                    cout << "Login successful. " << *p;  
                    patientMenu(p, docs, apts, bills, prs);
                    updateStatusBar();
                }
            }
            else if (role == 2) {
                Doctor* d = docs.getByID(id);
                if (d && d->login(id, pass)) {
                    auth = true;
                    updateStatusBar("Doctor", d->getName());
                    cout << "Login successful. " << *d; 
                    doctorMenu(d, pats, apts, bills, prs);
                    updateStatusBar();
                }
            }
            else if (role == 3) {
                Admin* a = admins.getByID(id);
                if (a && a->login(id, pass)) {
                    auth = true;
                    updateStatusBar("Admin", a->getName());
                    cout << "Login successful. " << *a;  
                    adminMenu(a, docs, pats, apts, bills, prs);
                    updateStatusBar();
                }
            }
            if (!auth) {
                attempts++;
                FileHandler::logSecurity(roleLabel, id, "FAILED");
                if (attempts == 3)
                    cout << "Account locked after 3 failed attempts. Contact admin.\n";
                else
                    cout << "Invalid ID or password. Attempt " << attempts << " of 3.\n";
            }
        }
    }
}
int main() {
    Storage<Patient>pats(100);
    Storage<Doctor> docs(50);
    Storage<Admin> admins(10);
    Storage<Appointment> apts(500);
    Storage<Bill> bills(500);
    Storage<Prescription> prs(500);
    showSplash();                        
    FileHandler::ensureFilesExist();
    if (!FileHandler::fileHasData("admin.txt"))
        setupFirstAdmin(admins);
    loadAllData(pats, docs, admins, apts, bills, prs);
    if (docs.getCount() == 0)
        setupInitialDoctors(docs);
    runLoginLoop(pats, docs, admins, apts, bills, prs);
    if (statusWin.isOpen())
        statusWin.close();
    cout << "Thank you for using MediCore. Goodbye!\n";
    return 0;
}