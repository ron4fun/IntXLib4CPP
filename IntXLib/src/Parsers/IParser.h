#pragma once

#ifndef IPARSER_H
#define IPARSER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <string>
#include <vector>
#include "../Utils/Dictionary.h"
#include "IntX.h"

using namespace std;

// Parser class interface.
 class IParser
{
public:
	
	virtual IntX Parse(const string &value, const UInt32 numberBase, const Dictionary &charToDigits, bool checkFormat) = 0;

	virtual UInt32 Parse(const string &value, int startIndex, int endIndex, const UInt32 numberBase, const Dictionary &charToDigits, std::vector<UInt32> &digitsRes) = 0;

}; // end class IParser

#endif // !IPARSER_H