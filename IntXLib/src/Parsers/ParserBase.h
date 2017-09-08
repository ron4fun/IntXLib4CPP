#pragma once

#ifndef PARSERBASE_H
#define PARSERBASE_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <string>
#include <math.h>
#include <vector>

#include "IParser.h"
#include "Pow2Parser.h"
#include "../Utils/Constants.h"
#include "../Utils/Dictionary.h"
#include "IntX.h"
#include "Bits.h"
#include "../Utils/Dictionary.h"

using namespace std;

class ParserBase : public IParser
{
public:
	
	/// <summary>
	/// Creates new <see cref="ParserBase" /> instance.
	/// </summary>
	/// <param name="pow2Parser">Parser for pow2 case.</param>
	ParserBase(IParser &pow2Parser)
	{
		_pow2Parser = &pow2Parser;
	} // end Constructor

	/// <summary>
	/// Parses provided string representation of <see cref="IntX" /> object.
	/// </summary>
	/// <param name="value">Number as string.</param>
	/// <param name="numberBase">Number base.</param>
	/// <param name="charToDigits">Char->digit dictionary.</param>
	/// <param name="checkFormat">Check actual format of number (0 or 0x at start).</param>
	/// <returns>Parsed object.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	/// <exception cref="ArgumentException"><paramref name="numberBase" /> is less then 2 or more then 16.</exception>
	/// <exception cref="FormatException"><paramref name="value" /> is not in valid format.</exception>
	virtual IntX Parse(const string &value, const UInt32 NumberBase, const Dictionary &charToDigits, bool checkFormat)
	{
		UInt32 numberBase = NumberBase;

		// Exceptions
		if (value.empty())
		{
			throw ArgumentNullException("value");
		} // end if
		if (charToDigits.size() == 0)
		{
			throw ArgumentNullException("charToDigits");
		} // end if
		if (numberBase < 2 || numberBase > charToDigits.size())
		{
			throw ArgumentException(string(Strings::ParseBaseInvalid) + "numberBase");
		} // end if

		// Initially determine start and end indices (Trim is too slow)
		int startIndex = 0;
		for (; (UInt32)startIndex < value.length() && value[startIndex] == ' '; ++startIndex);
		int endIndex = value.length() - 1;
		for (; endIndex >= startIndex && value[endIndex] == ' '; --endIndex);

		bool negative = false; // number sign
		bool stringNotEmpty = false; // true if string is already guaranteed to be non-empty

		int matchLength = 0;
		char firstPart = value[startIndex], hold;

		if (firstPart == '-')
		{
			negative = true;
			++matchLength;
		} // end if
		else if (firstPart == '+') 
		{
			negative = false;
			++matchLength;
		} // end else if

		if (firstPart == '$' || firstPart == '0')
		{
			hold = value[startIndex + 1];
			if (firstPart == '$')
			{
				if (checkFormat)
				{
					// '$' before the number - this is hex number  // Pascal Style
					numberBase = UInt32(16);
					++matchLength;
				} // end if
				else
					// This is an error
					throw FormatException(Strings::ParseInvalidChar);
			} // end if
			else if (hold == 'x' || hold == 'X')
			{
				if (checkFormat)
				{
					// '0x' before the number - this is hex number  // C Style
					numberBase = UInt32(16);
					matchLength += 2;
				} // end if
				else
					// This is an error
					throw FormatException(Strings::ParseInvalidChar);
			} // end else if
			else if (firstPart == '0')
			{
				if (checkFormat)
				{
					// 0 before the number - this is octal number
					numberBase = UInt32(8);
					++matchLength;
				} // end if
				else
					// This is an error
					throw FormatException(Strings::ParseInvalidChar);

				stringNotEmpty = true;
			} // end else if
						
		} // end if
		
		 // Skip leading sign and format
		startIndex += matchLength;

		// If on this stage string is empty, this may mean an error
		if (startIndex > endIndex && !stringNotEmpty)
		{
			throw FormatException(Strings::ParseNoDigits);
		} // end if

		// Iterate thru string and skip all leading zeroes
		for (; startIndex <= endIndex && value[startIndex] == '0'; ++startIndex);

		// Return zero if length is zero
		if (startIndex > endIndex) return IntX();

		// Determine length of new digits array and create new IntX object with given length
		int valueLength = endIndex - startIndex + 1; 
		UInt32 digitsLength = (UInt32)ceil(log(numberBase) / Constants::DigitBaseLog * valueLength);
		IntX newInt = IntX(digitsLength, negative);

		// Now we have only (in)valid string which consists from numbers only.
		// Parse it
		newInt.length = Parse(value, startIndex, endIndex, numberBase, charToDigits, newInt.digits);

		return newInt;
	} // end function Parse

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
		// Default implementation - always call pow2 parser if numberBase is pow of 2
		return numberBase == 1U << Bits::Msb(numberBase)
			? _pow2Parser->Parse(value, startIndex, endIndex, numberBase, charToDigits, digitsRes)
			: 0;
	} // end function Parse

private:
	IParser *_pow2Parser; // parser for pow2 case

}; // end class ParserBase

#endif // !PARSERBASE_H
