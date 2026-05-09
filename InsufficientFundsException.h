#pragma once
#include<iostream>
#include"HospitalException.h"
using namespace std;
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException();
};