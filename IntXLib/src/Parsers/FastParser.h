#pragma once

#ifndef FASTPARSER_H
#define FASTPARSER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IParser.h"
#include "ParserBase.h"
#include "../Utils/Dictionary.h"
#include "../Utils/Constants.h"
#include "Bits.h"
#include "../OpHelpers/StrRepHelper.h"
#include "../OpHelpers/DigitHelper.h"
#include "../OpHelpers/DigitOpHelper.h"
#include "../Multipliers/IMultiplier.h"
#include "../Multipliers/MultiplyManager.h"

#include <vector>

using namespace std;

class FastParser : public ParserBase
{
public:

	/// <summary>
	/// Creates new <see cref="FastParser" /> instance.
	/// </summary>
	/// <param name="pow2Parser">Parser for pow2 case.</param>
	/// <param name="classicParser">Classic parser.</param>
	FastParser(IParser &pow2Parser, IParser &classicParser) : ParserBase(pow2Parser)
	{
		_classicParser = &classicParser;
	} // end cctr

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
	virtual UInt32 Parse(const string &value, int startIndex, int endIndex, const UInt32 numberBase, const Dictionary &charToDigits, std::vector<UInt32> &digitsRes)
	{
		UInt32 newLength = ParserBase::Parse(value, startIndex, endIndex, numberBase, charToDigits, digitsRes);

		// Maybe base method already parsed this number
		if (newLength != 0) return newLength;

		// Check length - maybe use classic parser instead
		UInt32 initialLength = digitsRes.size();
		if (initialLength < Constants::FastParseLengthLowerBound || initialLength > Constants::FastParseLengthUpperBound)
		{
			return _classicParser->Parse(value, startIndex, endIndex, numberBase, charToDigits, digitsRes);
		} // end if

		UInt32 valueLength = (UInt32)(endIndex - startIndex + 1);
		UInt32 digitsLength = 1U << Bits::CeilLog2(valueLength);

		// Prepare array for digits in other base
		vector<UInt32> valueDigits(digitsLength);

		// This second array will store integer lengths initially
		vector<UInt32> valueDigits2(digitsLength);

		UInt32 * const valueDigitsStartPtr = &valueDigits[0];
		UInt32 * const valueDigitsStartPtr2 = &valueDigits2[0];

		// In the string first digit means last in digits array
		UInt32* valueDigitsPtr = valueDigitsStartPtr + valueLength - 1;
		UInt32* valueDigitsPtr2 = valueDigitsStartPtr2 + valueLength - 1;

		// Reverse copy characters into digits
		const char* valueStartPtr = &value[0];
	
		const char* valuePtr = valueStartPtr + startIndex;
		const char* valueEndPtr = valuePtr + valueLength;
		for (; valuePtr < valueEndPtr; ++valuePtr, --valueDigitsPtr, --valueDigitsPtr2)
		{
			// Get digit itself - this call will throw an exception if char is invalid
			*valueDigitsPtr = StrRepHelper::GetDigit(charToDigits, *valuePtr, numberBase);

			// Set length of this digit (zero for zero)
			*valueDigitsPtr2 = *valueDigitsPtr == 0U ? 0U : 1U;
		} // end for
		

		// We have retrieved lengths array from pool - it needs to be cleared before using
		DigitHelper::SetBlockDigits(valueDigitsStartPtr2 + valueLength, digitsLength - valueLength, 0);

		// Now start from the digit arrays beginning
		valueDigitsPtr = valueDigitsStartPtr;
		valueDigitsPtr2 = valueDigitsStartPtr2;

		// Current multiplier (classic or fast) will be used
		IMultiplier *multiplier = MultiplyManager::GetCurrentMultiplier();

		// Here base in needed power will be stored
		IntX baseInt = IntX(), null = IntX();

		// Variables used in cycle
		UInt32* ptr1, *ptr2, *valueDigitsPtrEnd;
		UInt32 loLength, hiLength;

		// Outer cycle instead of recursion
		for (UInt32 innerStep = 1, outerStep = 2; innerStep < digitsLength; innerStep <<= 1, outerStep <<= 1)
		{
			if (IntX::ReferenceEquals(baseInt, null))
				baseInt = numberBase;
			else
				baseInt = baseInt * baseInt;

			// Start from arrays beginning
			ptr1 = valueDigitsPtr;
			ptr2 = valueDigitsPtr2;

			// vauleDigits array end
			valueDigitsPtrEnd = valueDigitsPtr + digitsLength;

			// Cycle thru all digits and their lengths
			for (; ptr1 < valueDigitsPtrEnd; ptr1 += outerStep, ptr2 += outerStep)
			{
				// Get lengths of "lower" and "higher" value parts
				loLength = *ptr2;
				hiLength = *(ptr2 + innerStep);

				if (hiLength != 0)
				{
					// We always must clear an array before multiply
					DigitHelper::SetBlockDigits(ptr2, outerStep, 0U);

					// Multiply per baseInt
					hiLength = multiplier->Multiply(
						&baseInt.digits[0], 
						baseInt.length,
						ptr1 + innerStep,
						hiLength,
						ptr2);
				} // end if

				// Sum results
				if (hiLength != 0 || loLength != 0)
				{
					*ptr1 = DigitOpHelper::Add(
						ptr2,
						hiLength,
						ptr1,
						loLength,
						ptr2);
				} // end if
				else
				{
					*ptr1 = 0U;
				} // end else
			} // end for

			// After inner cycle valueDigits will contain lengths and valueDigits2 will contain actual values
			// so we need to swap them here
			vector<UInt32> tempDigits = valueDigits;
			valueDigits = valueDigits2;
			valueDigits2 = tempDigits;

			//UInt32 *tempPtr = valueDigitsPtr;
			//valueDigitsPtr = valueDigitsPtr2;
			//valueDigitsPtr2 = tempPtr;
		} // end for	

		// Determine real length of converted number
		UInt32 realLength = valueDigits2[0];

		// Copy to result
		memcpy((UInt32 *)(&digitsRes[0]), (UInt32 *)(&valueDigits[0]), realLength * sizeof(UInt32));

		return realLength;
	} // end func Parse

private:
	IParser *_classicParser;

}; // end class FastParser

#endif // !FASTPARSER_H
