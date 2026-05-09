#pragma once
#include<iostream>
#include "HospitalException.h"
using namespace std;
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException();
};