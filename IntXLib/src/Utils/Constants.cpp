#include "stdafx.h"
#include "Constants.h"
#include <math.h>

// Initialize static variable
string Constants::BaseUpperChars = "0123456789ABCDEF";
string Constants::BaseLowerChars = "0123456789abcdef";
Dictionary Constants::BaseCharToDigits = Dictionary(Constants::fillBaseCharToDigits());

const double Constants::EulersNumber = 2.7182818284590451;
const double Constants::DigitBaseLog = log(4294967296L);
const double Constants::PI = 3.1415926535897931;
const int Constants::MinIntValue = (int)(2147483648L) * -1L;
const __int64 Constants::MinInt64Value = -9223372036854775808LL;