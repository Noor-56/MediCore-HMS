#pragma once
#include<iostream>
using namespace std;
class HospitalException {
    char message[200];
public:
    HospitalException(const char* msg);
    virtual const char* what() const;
    virtual ~HospitalException();
};