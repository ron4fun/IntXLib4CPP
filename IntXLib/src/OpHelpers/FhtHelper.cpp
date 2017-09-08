#include "stdafx.h"
#include "FhtHelper.h"
#include <math.h>

// Initialize static variable
const int FhtHelper::DoubleDataLengthShift = 2 - (FhtHelper::DoubleDataBytes >> 1);
const int FhtHelper::DoubleDataDigitShift = FhtHelper::DoubleDataBytes << 3;
const long long FhtHelper::DoubleDataBaseInt = 1LL << FhtHelper::DoubleDataDigitShift;
const double FhtHelper::FhtHelper::DoubleDataBase = FhtHelper::DoubleDataBaseInt;
const double FhtHelper::DoubleDataBaseDiv2 = FhtHelper::DoubleDataBase / 2.0;

const double FhtHelper::Sqrt2 = sqrt(2.0);
const double FhtHelper::Sqrt2Div2 = Sqrt2 / 2.0;
vector<double> FhtHelper::SineTable = vector<double>(31);
bool FhtHelper::isSineTableInitialized = false;
//FhtHelper::TrigValues FhtHelper::trigValues = FhtHelper::TrigValues();