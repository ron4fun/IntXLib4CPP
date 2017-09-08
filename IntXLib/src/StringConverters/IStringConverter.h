#pragma once

#ifndef ISTRINGCONVERTER_H
#define ISTRINGCONVERTER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <string>
#include <vector>
#include "IntX.h"

using namespace std;

// ToString converter class interface.
class IStringConverter
{
public:
	
	/// <summary>
	/// Returns string representation of <see cref="IntX" /> object in given base.
	/// </summary>
	/// <param name="intX">Big integer to convert.</param>
	/// <param name="numberBase">Base of system in which to do output.</param>
	/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
	/// <returns>Object string representation.</returns>
	virtual string ToString(const IntX &intX, const UInt32 numberBase, const string &alphabet) = 0;

	/// <summary>
	/// Converts digits from internal representaion into given base.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="numberBase">Base to use for output.</param>
	/// <param name="outputLength">Calculated output length (will be corrected inside).</param>
	/// <returns>Conversion result (later will be transformed to string).</returns>
	virtual vector<UInt32> ToString(const vector<UInt32> &digits, const UInt32 length, const UInt32 numberBase, UInt32 &outputLength) = 0;
	
}; // end class IStringConverter

#endif // !ISTRINGCONVERTER_H
