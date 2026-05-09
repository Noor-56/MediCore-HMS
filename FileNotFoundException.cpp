#include"FileNotFoundException.h"

FileNotFoundException::FileNotFoundException()
    : HospitalException("Error: Could not open required file.") {
}