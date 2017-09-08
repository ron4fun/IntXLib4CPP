#pragma once

#ifndef CLASSICSTRINGCONVERTER_H
#define CLASSICSTRINGCONVERTER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "StringConverterBase.h"
#include <vector>
#include <stdlib.h>

using namespace std;

// Classic ToString converting algorithm using division (O[n^2]).
class ClassicStringConverter : public StringConverterBase
{
	
	/// <summary>
	/// Creates new <see cref="ClassicStringConverter" /> instance.
	/// </summary>
	/// <param name="pow2StringConverter">Converter for pow2 case.</param>
	public: ClassicStringConverter(IStringConverter &pow2StringConverter) : StringConverterBase(pow2StringConverter) {}

public:
	
	/// <summary>
	/// Converts digits from internal representation into given base.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="numberBase">Base to use for output.</param>
	/// <param name="outputLength">Calculated output length (will be corrected inside).</param>
	/// <returns>Conversion result (later will be transformed to string).</returns>
	virtual vector<UInt32> ToString(const vector<UInt32> &digits, const UInt32 length, const UInt32 numberBase, UInt32 &outputLength)
	{
		vector<UInt32> outputArray = StringConverterBase::ToString(digits, length, numberBase, outputLength);

		// Maybe base method already converted this number
		if (!outputArray.empty()) return outputArray;

		// Create an output array for storing of number in other base
		outputArray = vector<UInt32>(outputLength + 1);

		// Make a copy of initial data
		vector<UInt32> digitsCopy(digits);
		//memcpy(&digitsCopy[0], &digits[0], length * sizeof(unsigned int));

		// Calculate output numbers by dividing
		UInt32 outputIndex;
		UInt32 lengthCopy = length;
		for (outputIndex = 0; lengthCopy > 0; ++outputIndex)
		{
			lengthCopy = DigitOpHelper::DivMod((UInt32*)&digitsCopy[0], lengthCopy, numberBase, (UInt32*)&digitsCopy[0], outputArray[outputIndex]);
		} // end for

		outputLength = outputIndex;
		return outputArray;
	} // end function ToString

}; // end class ClassicStringConverter

#endif // !CLASSICSTRINGCONVERTER_H
