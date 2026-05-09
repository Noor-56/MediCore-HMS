#pragma once
#include"HospitalException.h"
#include<iostream>
using namespace std;
class InvalidInputException : public HospitalException {
public:
    InvalidInputException();
};