#pragma once

#ifndef CLASSICPARSER_H
#define CLASSICPARSER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "ParserBase.h"
#include "IParser.h"
#include "../Utils/Dictionary.h"
#include "../OpHelpers/StrRepHelper.h"
#include "../Utils/Utils.h"
#include <vector>

using namespace std;

class ClassicParser : public ParserBase
{
public:
	
	/// <summary>
	/// Creates new <see cref="ClassicParser" /> instance.
	/// </summary>
	/// <param name="pow2Parser">Parser for pow2 case.</param>
	ClassicParser(IParser &pow2Parser) : ParserBase(pow2Parser) {}

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

		// Do parsing in big cycle
		UInt64 numberBaseLong = numberBase;
		UInt64 digit;
		for (int i = startIndex; i <= endIndex; ++i)
		{
			digit = StrRepHelper::GetDigit(charToDigits, value[i], numberBase);

			// Next multiply existing values by base and add this value to them
			if (newLength == 0)
			{
				if (digit != 0)
				{
					digitsRes[0] = (UInt32)digit;
					newLength = 1;
				} // end if
			} // end if
			else
			{
				for (UInt32 j = 0; j < newLength; ++j)
				{
					digit += UInt64(digitsRes[j]) * numberBaseLong;
					digitsRes[j] = (UInt32)digit;
					digit >>= 32;
				} // end for
				if (digit != 0)
				{
					digitsRes[newLength++] = (UInt32)digit;
				} // end if
			} // end else
		} // end for

		return newLength;
	} // end func Parse

}; // end class ClassicParser

#endif // !CLASSICPARSER_H
