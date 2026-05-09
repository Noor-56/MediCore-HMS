#include"FileHandler.h"
string FileHandler::todayString() {
    time_t now = time(nullptr);
    struct tm t;
    localtime_s(&t, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%d-%m-%Y", &t);
    return string(buf);
}
void FileHandler::ensureFilesExist() {
    const char* files[] = { "patients.txt","doctors.txt","admin.txt","appointments.txt","bills.txt","prescriptions.txt","security_log.txt","discharged.txt" };
    for (int i = 0; i < 8; i++) {
        ifstream f(files[i]);
        if (!f.is_open()) {
            ofstream o(files[i]);
        }
    }
}
bool FileHandler::fileHasData(const string& filename) {
    ifstream f(filename);
    if (!f.is_open())
        return false;
    string line;
    while (getline(f, line))
        if (!line.empty())
            return true;
    return false;
}
void FileHandler::loadPatients(Storage<Patient>& s) {
    ifstream f("patients.txt");
    if (!f.is_open())
        throw FileNotFoundException();
    string line;
    while (getline(f, line)) {
        if (line.empty())
            continue;
        istringstream ss(line);
        string id, nm, age, gen, cont, pass, bal;
        getline(ss, id, ',');
        getline(ss, nm, ',');
        getline(ss, age, ',');
        getline(ss, gen, ',');
        getline(ss, cont, ',');
        getline(ss, pass, ',');
        getline(ss, bal, ',');
        if (id.empty())
            continue;
        s.add(new Patient(stoi(id), nm, pass, stoi(age), gen[0], stof(bal), cont));
    }
}
void FileHandler::loadDoctors(Storage<Doctor>& s) {
    ifstream f("doctors.txt");
    if (!f.is_open())
        throw FileNotFoundException();
    string line;
    while (getline(f, line)) {
        if (line.empty())
            continue;
        istringstream ss(line);
        string id, nm, spec, cont, pass, fee;
        getline(ss, id, ',');
        getline(ss, nm, ',');
        getline(ss, spec, ',');
        getline(ss, cont, ',');
        getline(ss, pass, ',');
        getline(ss, fee, ',');
        if (id.empty())
            continue;
        s.add(new Doctor(stoi(id), nm, pass, spec, stof(fee), cont));
    }
}
void FileHandler::loadAdmins(Storage<Admin>& s) {
    ifstream f("admin.txt");
    if (!f.is_open())
        throw FileNotFoundException();
    string line;
    while (getline(f, line)) {
        if (line.empty())
            continue;
        istringstream ss(line);
        string id, nm, pass;
        getline(ss, id, ',');
        getline(ss, nm, ',');
        getline(ss, pass, ',');
        if (id.empty())
            continue;
        s.add(new Admin(stoi(id), nm, pass));
    }
}
void FileHandler::loadAppointments(Storage<Appointment>& s) {
    ifstream f("appointments.txt");
    if (!f.is_open())
        return;
    string line;
    while (getline(f, line)) {
        if (line.empty())
            continue;
        istringstream ss(line);
        string aid, pid, did, dstr, tstr, stat;
        getline(ss, aid, ',');
        getline(ss, pid, ',');
        getline(ss, did, ',');
        getline(ss, dstr, ',');
        getline(ss, tstr, ',');
        getline(ss, stat, ',');
        if (aid.empty())
            continue;
        int d = stoi(dstr.substr(0, 2)), mo = stoi(dstr.substr(3, 2)), y = stoi(dstr.substr(6, 4));
        int h = stoi(tstr.substr(0, 2)), mi = stoi(tstr.substr(3, 2));
        s.add(new Appointment(stoi(aid), stoi(pid), stoi(did), d, mo, y, h, mi, stat));
    }
}
void FileHandler::loadBills(Storage<Bill>& s) {
    ifstream f("bills.txt");
    if (!f.is_open())
        return;
    string line;
    while (getline(f, line)) {
        if (line.empty())
            continue;
        istringstream ss(line);
        string bid, pid, aid, amt, stat, dstr;
        getline(ss, bid, ',');
        getline(ss, pid, ',');
        getline(ss, aid, ',');
        getline(ss, amt, ',');
        getline(ss, stat, ',');
        getline(ss, dstr, ',');
        if (bid.empty())
            continue;
        int d = stoi(dstr.substr(0, 2)), mo = stoi(dstr.substr(3, 2)), y = stoi(dstr.substr(6, 4));
        Bill* b = new Bill(stoi(bid), stoi(aid), stoi(pid), d, mo, y, stof(amt));
        b->setStatus(stat);
        s.add(b);
    }
}
void FileHandler::loadPrescriptions(Storage<Prescription>& s) {
    ifstream f("prescriptions.txt");
    if (!f.is_open())
        return;
    string line;
    while (getline(f, line)) {
        if (line.empty())
            continue;
        istringstream ss(line);
        string prid, aid, pid, did, dstr, meds, notes;
        getline(ss, prid, ',');
        getline(ss, aid, ',');
        getline(ss, pid, ',');
        getline(ss, did, ',');
        getline(ss, dstr, ',');
        getline(ss, meds, ',');
        getline(ss, notes, ',');
        if (prid.empty())
            continue;
        int d = stoi(dstr.substr(0, 2)), mo = stoi(dstr.substr(3, 2)), y = stoi(dstr.substr(6, 4));
        s.add(new Prescription(stoi(prid), stoi(aid), stoi(pid), stoi(did), d, mo, y, "", meds, notes));
    }
}
void FileHandler::appendLine(const string& filename, const string& csv) {
    ofstream f(filename, ios::app);
    if (!f.is_open())
        throw FileNotFoundException();
    f << csv << "\n";
}
void FileHandler::rewritePatients(Storage<Patient>& s) {
    ofstream f("patients.txt");
    for (int i = 0; i < s.getCount(); i++)
        f << s.get(i)->toCSV() << "\n";
}
void FileHandler::rewriteDoctors(Storage<Doctor>& s) {
    ofstream f("doctors.txt");
    for (int i = 0; i < s.getCount(); i++)
        f << s.get(i)->toCSV() << "\n";
}
void FileHandler::rewriteAppointments(Storage<Appointment>& s) {
    ofstream f("appointments.txt");
    for (int i = 0; i < s.getCount(); i++)
        f << s.get(i)->toCSV() << "\n";
}
void FileHandler::rewriteBills(Storage<Bill>& s) {
    ofstream f("bills.txt");
    for (int i = 0; i < s.getCount(); i++)
        f << s.get(i)->toCSV() << "\n";
}
void FileHandler::rewritePrescriptions(Storage<Prescription>& s) {
    ofstream f("prescriptions.txt");
    for (int i = 0; i < s.getCount(); i++)
        f << s.get(i)->toCSV() << "\n";
}
int FileHandler::maxPatientID(Storage<Patient>& s) {
    int mx = 0;
    for (int i = 0; i < s.getCount(); i++)
        if (s.get(i)->getID() > mx)
            mx = s.get(i)->getID();
    return mx;
}
int FileHandler::maxDocID(Storage<Doctor>& s) {
    int mx = 0;
    for (int i = 0; i < s.getCount(); i++)
        if (s.get(i)->getID() > mx)
            mx = s.get(i)->getID();
    return mx;
}
int FileHandler::maxApptID(Storage<Appointment>& s) {
    int mx = 0;
    for (int i = 0; i < s.getCount(); i++)
        if (s.get(i)->getID() > mx)
            mx = s.get(i)->getID();
    return mx;
}
int FileHandler::maxBillID(Storage<Bill>& s) {
    int mx = 0;
    for (int i = 0; i < s.getCount(); i++)
        if (s.get(i)->getID() > mx)
            mx = s.get(i)->getID();
    return mx;
}
int FileHandler::maxPresID(Storage<Prescription>& s) {
    int mx = 0;
    for (int i = 0; i < s.getCount(); i++)
        if (s.get(i)->getID() > mx)
            mx = s.get(i)->getID();
    return mx;
}
void FileHandler::logSecurity(const string& role, int id, const string& result) {
    time_t now = time(nullptr);
    struct tm t;
    localtime_s(&t, &now);
    char buf[30];
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", &t);
    ofstream f("security_log.txt", ios::app);
    f << buf << "," << role << "," << id << "," << result << "\n";
}
void FileHandler::archivePatient(Patient* p, Storage<Appointment>& apts, Storage<Prescription>& prs, Storage<Bill>& bls) {
    ofstream f("discharged.txt", ios::app);
    f << p->toCSV() << "\n";
    for (int i = 0; i < apts.getCount(); i++)
        if (apts.get(i)->getPatientID() == p->getID())
            f << apts.get(i)->toCSV() << "\n";
    for (int i = 0; i < prs.getCount(); i++)
        if (prs.get(i)->getPatientID() == p->getID())
            f << prs.get(i)->toCSV() << "\n";
    for (int i = 0; i < bls.getCount(); i++)
        if (bls.get(i)->getPatientID() == p->getID())
            f << bls.get(i)->toCSV() << "\n";
}
