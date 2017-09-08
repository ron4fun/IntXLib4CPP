#pragma once

#ifndef POW2PARSER_H
#define POW2PARSER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IParser.h"
#include "../Utils/Dictionary.h"
#include "../Utils/Constants.h"
#include "Bits.h"
#include "../OpHelpers/StrRepHelper.h"

#include "IntX.h"

#include <string>
#include <vector>

using namespace std;

class Pow2Parser : public IParser
{
public:
	// Not needed in this implementation
	virtual IntX Parse(const string &value, const UInt32 numberBase, const Dictionary &charToDigits, bool checkFormat)
	{
		return IntX();
	} // end func Parse

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="startIndex">Index inside string from which to start.</param>
	/// <param name="endIndex">Index inside string on which to end.</param>
	/// <param name="numberBase">Number base.</param>
	/// <param name="charToDigits">Char->digit dictionary.</param>
	/// <param name="digitsRes">Resulting digits.</param>
	/// <returns>Parsed integer length.</returns>
	virtual UInt32 Parse(const string &value, int startIndex, int endIndex, const UInt32 numberBase, const Dictionary &charToDigits, vector<UInt32> &digitsRes)
	{
		// Calculate length of input string
		int bitsInChar = Bits::Msb(numberBase);
		UInt32 valueLength = (unsigned int)(endIndex - startIndex + 1);
		UInt64 valueBitLength = (UInt64)valueLength * (UInt64)bitsInChar;

		// Calculate needed digits length and first shift
		UInt32 digitsLength = (UInt32)(valueBitLength / Constants::DigitBitCount) + 1;
		UInt32 digitIndex = digitsLength - 1;
		int initialShift = (int)(valueBitLength % Constants::DigitBitCount);

		// Probably correct digits length
		if (initialShift == 0)
		{
			--digitsLength;
		} // end if

		// Do parsing in big cycle
		UInt32 digit;
		for (int i = startIndex; i <= endIndex; ++i)
		{
			digit = StrRepHelper::GetDigit(charToDigits, value[i], numberBase);

			// Correct initial digit shift
			if (initialShift == 0)
			{
				// If shift is equals to zero then char is not on digit elemtns bounds,
				// so just go to the previous digit
				initialShift = Constants::DigitBitCount - bitsInChar;
				--digitIndex;
			} // end if
			else
			{
				// Here shift might be negative, but it's okay
				initialShift -= bitsInChar;
			} // end else

			// Insert new digit in correct place
			digitsRes[digitIndex] |= initialShift < 0 ? digit >> -initialShift : digit << initialShift;

			// In case if shift was negative we also must modify previous digit
			if (initialShift < 0)
			{
				initialShift += Constants::DigitBitCount;
				digitsRes[--digitIndex] |= digit << initialShift;
			} // end if
		} // end for

		if (digitsRes[digitsLength - 1] == 0)
		{
			--digitsLength;
		} // end if

		return digitsLength;
	} // end func Parse

}; // end class Pow2Parser

#endif // !POW2PARSER_H
