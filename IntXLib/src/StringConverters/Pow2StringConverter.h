#pragma once

#ifndef POW2STRINGCONVERTER_H
#define POW2STRINGCONVERTER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IStringConverter.h"
#include <string>
#include <vector>
#include "IntX.h"
#include "Bits.h"
#include "../Utils/Constants.h"

using namespace std;

// Provides special fast (with linear time) ToString converting if base is pow of 2.
class Pow2StringConverter : public IStringConverter
{
public:
	// Not needed in this implementation
	virtual string ToString(const IntX &intX, const UInt32 numberBase, const string &alphabet)
	{
		return "";
	} // end function ToString

	/// <summary>
	/// Converts digits from internal representaion into given base.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="numberBase">Base to use for output.</param>
	/// <param name="outputLength">Calculated output length (will be corrected inside).</param>
	/// <returns>Conversion result (later will be transformed to string).</returns>
	virtual vector<UInt32> ToString(const vector<UInt32> &digits, const UInt32 length, const UInt32 numberBase, UInt32 &outputLength)
	{
		// Calculate real output length
		int bitsInChar = Bits::Msb(numberBase);
		UInt64 digitsBitLength = (UInt64)(length - 1) * Constants::DigitBitCount + (UInt64)Bits::Msb(digits[length - 1]) + 1UL;
		UInt32 realOutputLength = (UInt32)(digitsBitLength / (UInt64)bitsInChar);
		if (digitsBitLength % (UInt64)bitsInChar != 0)
		{
			++realOutputLength;
		} // end if

		// Prepare shift variables
		int nextDigitShift = Constants::DigitBitCount - bitsInChar; // after this shift next digit must be used
		int initialShift = 0;

		// We will also need bitmask for copying digits
		UInt32 digitBitMask = numberBase - 1;

		// Create an output array for storing of number in other base
		vector<UInt32> outputArray(realOutputLength);

		// Walk thru original digits and fill output
		UInt32 outputDigit;
		for (UInt32 outputIndex = 0, digitIndex = 0; outputIndex < realOutputLength; ++outputIndex)
		{
			// Get part of current digit
			outputDigit = digits[digitIndex] >> initialShift;

			// Maybe we need to go to the next digit
			if (initialShift >= nextDigitShift)
			{
				// Go to the next digit
				++digitIndex;

				// Maybe we also need a part of the next digit
				if (initialShift != nextDigitShift && digitIndex < length)
				{
					outputDigit |= digits[digitIndex] << (Constants::DigitBitCount - initialShift);
				} // end if

				// Modify shift so that it will be valid for the next digit
				initialShift = (initialShift + bitsInChar) % Constants::DigitBitCount;
			} // end if
			else
			{
				// Modify shift as usual
				initialShift += bitsInChar;
			} // end else

			// Write masked result to the output
			outputArray[outputIndex] = outputDigit & digitBitMask;
		} // end for

		outputLength = realOutputLength;
		return outputArray;
	} // end ToString

}; // end class Pow2StringConverter

#endif // !POW@STRINGCONVERTER_H
