#include "SlotUnavailableException.h"
SlotUnavailableException::SlotUnavailableException()
    : HospitalException("Error: That time slot is already taken.") {
}