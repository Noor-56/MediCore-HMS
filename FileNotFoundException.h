#pragma once
#include "HospitalException.h"
#include<iostream>
using namespace std;
class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException();
};