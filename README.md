"# MediCore-HMS" 
**Course:** Object Oriented Programming  
**Semester:** Spring 2026   
**Student Name:** Noor ul Huda 
**Roll Number:** 25L-0851
---

## 🔗 GitHub Repository
**Link:** 
https://github.com/Noor-56/MediCore-HMS
---

## Project Overview
MediCore is a Hospital Management System developed for the Spring 2026 OOP course. It manages the workflows for Patients, Doctors, and Administrators, including appointment scheduling, automated billing, and medical record persistence. The system utilizes SFML for a graphical status interface and follows strict modular programming standards.

---

## Compilation & Execution Instructions

### Prerequisites
1. **IDE:** Visual Studio 2022.
2. **Library:** **SFML**.
3. **Language:** C++

### Setup Steps
1. **Include SFML:** Link the SFML `include` and `lib` directories in Project Properties.
2. **Dependencies:** Add `sfml-graphics.lib`, `sfml-window.lib`, and `sfml-system.lib` to the Linker Input.
3. **Assets:** Ensure `segoeui.ttf` or `arial.ttf` is available in the Windows Fonts directory (or project folder) for the SFML status bar.
4. **Run:** Build the solution and run via `Ctrl + F5`.

---

## Key Implementation Details
- **String Management:** Utilized `std::string` for robust text handling and member data.
- **Custom Storage Engine:** Implemented a generic `Storage<T>` template class. **No STL vectors** were used, adhering to Rule VI.
- **Manual Logic:** While `std::string` was used for storage, logic such as case-insensitive searching and sorting (Date Ascending/Descending) was implemented manually without `std::sort`.
- **Memory Management:** Strictly managed dynamic memory for all object pointers within the `Storage` class to ensure zero memory leaks.
- **File I/O:** All data persists across sessions via `.txt` files handled by a specialized `FileHandler` class.

---

## Project Structure
- **Core Entities:** `Patient`, `Doctor`, `Admin`, `Appointment`, `Bill`, `Prescription`.
- **Infrastructure:** `Storage<T>` (Custom Template), `FileHandler` (Persistence), `Validator` (Input Checking).
- **UI:** `main.cpp` (Sequence of calls), SFML Splash and Status Bar functions.

---

## ⚖️ Minimum Required Overloads
- `==` (Appointment): Detects scheduling overlaps.
- `<<` (Patient/Doctor/Appointment): Streams formatted data to console.
- `-=` (Patient): Deducts fees from balance.
- `==` (Patient/Doctor): Compares entities by unique ID.
