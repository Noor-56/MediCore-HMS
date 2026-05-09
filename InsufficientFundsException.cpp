#include"InsufficientFundsException.h"

InsufficientFundsException::InsufficientFundsException()
	: HospitalException("Error: Insufficient balance.") {
}