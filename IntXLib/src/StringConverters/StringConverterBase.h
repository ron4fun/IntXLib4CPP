#pragma once

#ifndef STRINGCONVERTERBASE_H
#define STRINGCONVERTERBASE_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IStringConverter.h"
#include "Pow2StringConverter.h"
#include "../Utils/Constants.h"
#include "Bits.h"
#include <string>
#include <vector>
#include <math.h>

using namespace std;

// Base class for ToString converters.
// Contains default implementations of convert operation over <see cref="IntX" /> instances.
class StringConverterBase : public IStringConverter
{
	#pragma region Private fields
private:
		IStringConverter *_pow2StringConverter; // converter for pow2 case

	#pragma endregion Private fields

	#pragma region Constructor

public:
	
	// Creates new <see cref="StringConverterBase" /> instance.
	// <param name="pow2StringConverter">Converter for pow2 case.</param>
	StringConverterBase(IStringConverter &pow2StringConverter)
	{
		_pow2StringConverter = &pow2StringConverter;
	} // end cctor

	#pragma endregion Constructor

public:
	
	/// <summary>
	/// Returns string representation of <see cref="IntX" /> object in given base.
	/// </summary>
	/// <param name="intX">Big integer to convert.</param>
	/// <param name="numberBase">Base of system in which to do output.</param>
	/// <param name="alphabet">Alphabet which contains chars used to represent big integer, char position is coresponding digit value.</param>
	/// <returns>Object string representation.</returns>
	/// <exception cref="ArgumentException"><paramref name="numberBase" /> is less then 2 or <paramref name="intX" /> is too big to fit in string.</exception>
	virtual string ToString(const IntX &intX, const UInt32 numberBase, const string &alphabet)
	{
		// Test base
		if (numberBase < 2 || numberBase > 65536)
		{
			throw ArgumentException(string(Strings::ToStringSmallBase) + "numberBase");
		} // end if

		// Special processing for zero values
		if (intX.length == 0) return "0";

		// Calculate output array length
		UInt32 outputLength = (UInt32)ceil(Constants::DigitBaseLog / log(numberBase) * intX.length);

		// Define length coefficient for string builder
		bool isBigBase = numberBase > alphabet.length();
		UInt32 lengthCoef = isBigBase ? (UInt32)ceil(log10(numberBase)) + 2U : 1U;

		// Determine maximal possible length of string
		UInt64 maxBuilderLength = (UInt64)outputLength * lengthCoef + 1UL;
		if (maxBuilderLength > Constants::MaxIntValue)
		{
			// This big integer can't be transformed to string
			throw ArgumentException(string(Strings::IntegerTooBig) + "intX");
		} // end if

		// Transform digits into another base
		vector<UInt32> outputArray = ToString(intX.digits, intX.length, numberBase, outputLength);

		// Output everything to the string builder
		string outputBuilder;
		outputBuilder.resize((int)(outputLength * lengthCoef + 1));
		
		UInt32 Idx = 0; // holds index in string
		UInt32 aCount = 0; // counts numbers of char in string

		// Maybe append minus sign
		if (intX.negative)
		{
			outputBuilder[Idx] += Constants::DigitsMinusChar;
			 ++Idx;
			++aCount;
		} // end if

		// Output all digits
		for (UInt32 i = outputLength - 1; i < outputLength; --i)
		{
			if (!isBigBase)
			{
				// Output char-by-char for bases up to covered by alphabet
				outputBuilder[Idx] += (alphabet[(int)outputArray[i]]);
				++aCount;
			} // end if
			else
			{
				// Output digits in bracets for bigger bases
				outputBuilder[Idx] += Constants::DigitOpeningBracet;
				++Idx;
				outputBuilder[Idx] += (char)(outputArray[i] + '0');
				++Idx;
				outputBuilder[Idx] += Constants::DigitClosingBracet;
				 aCount += 3;
			} // end else

			++Idx;
		} // end for
		
		outputBuilder.resize(aCount); // resize to correct size

		return outputBuilder;
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
		// Default implementation - always call pow2 converter if numberBase is pow of 2
		return numberBase == 1U << Bits::Msb(numberBase)
			? _pow2StringConverter->ToString(digits, length, numberBase, outputLength)
			: vector<UInt32>();
	} // end function ToString

}; // end class StringConverterBase

#endif // !STRINGCONVERTERBASE_H
