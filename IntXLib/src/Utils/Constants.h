#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <string>
#include "../OpHelpers/StrRepHelper.h"
#include "Dictionary.h"

using namespace std;

class Constants
{
public:
	static Dictionary fillBaseCharToDigits()
	{
		Dictionary baseCharToDigits = Dictionary(StrRepHelper::CharDictionaryFromAlphabet(BaseUpperChars, 16U));

		for (UInt32 i = 10; i < BaseLowerChars.length(); i++)
		{
		   (baseCharToDigits)[BaseLowerChars[i]] = (UInt32)i;
		} // end for

		return baseCharToDigits;
	} // end function fillBaseCharToDigits

	static Dictionary BaseCharToDigits;

	// Chars used to parse/output big integers (upper case).
	static string BaseUpperChars;

	// Chars used to parse/output big integers (lower case).
	static string BaseLowerChars;

	// Standard char->digit dictionary.
	//static IDictionary<char, uint> BaseCharToDigits;

	// Digit opening bracet (used for bases bigger then 16).
	static const char DigitOpeningBracet = '{';

	// Digit closing bracet (used for bases bigger then 16).
	static const char DigitClosingBracet = '}';

	// Minus char (-).
	static const char DigitsMinusChar = '-';

	// Natural logarithm of digits base (log(2^32)).
	static const double DigitBaseLog;


	// Minimal Log2(array length) which will be pooled using any array pool.
	static const UInt32 MinPooledArraySizeLog2 = 17;

	// Maximal Log2(array length) which will be pooled using any array pool.
	static const UInt32 MaxPooledArraySizeLog2 = 31;

	// Maximal allowed array pool items count in each stack.
	static const UInt32 MaxArrayPoolCount = 1024;


	// <see cref="IntX" /> length from which FHT is used (in auto-FHT mode).
	// Before this length usual multiply algorithm works faster.
	static const UInt32 AutoFhtLengthLowerBound = 512;

	// <see cref="IntX" /> length 'till which FHT is used (in auto-FHT mode).
	// After this length using of FHT may be unsafe due to big precision errors.
	static const UInt32 AutoFhtLengthUpperBound = 67108864;

	// Number of lower digits used to check FHT multiplication result validity.
	static const UInt32 FhtValidityCheckDigitCount = 10;


	// <see cref="IntX" /> length from which Newton approach is used (in auto-Newton mode).
	// Before this length usual divide algorithm works faster.
	static const UInt32 AutoNewtonLengthLowerBound = 8192;

	// <see cref="IntX" /> length 'till which Newton approach is used (in auto-Newton mode).
	// After this length using of fast division may be slow.
	static const UInt32 AutoNewtonLengthUpperBound = 67108864;
	
	// <see cref="IntX" /> length from which fast parsing is used (in Fast parsing mode).
	// Before this length usual parsing algorithm works faster.
	static const UInt32 FastParseLengthLowerBound = 32;

	// <see cref="IntX" /> length 'till which fast parsing is used (in Fast parsing mode).
	// After this length using of parsing will be slow.
	static const UInt32 FastParseLengthUpperBound = 4294967295;

	// <see cref="IntX" /> length from which fast convertion is used (in Fast convert mode).
	// Before this length usual convertion algorithm works faster.
	static const UInt32 FastConvertLengthLowerBound = 16;

	// <see cref="IntX" /> length 'till which fast convertion is used (in Fast convert mode).
	// After this length using of convertion will be slow.
	static const UInt32 FastConvertLengthUpperBound = 4294967295;


	// Count of bits in one <see cref="IntX" /> digit.
	static const UInt32 DigitBitCount = 32;

	// Maximum count of bits which can fit in <see cref="IntX" />.
	static const UInt64 MaxBitCount = 137438953440L;

	// 2^<see cref="DigitBitCount"/>.
	static const UInt64 BitCountStepOf2 = 4294967296L;

	/// <summary>
	/// Euler's Number.
	/// </summary>
	static const double EulersNumber; // = 2.7182818284590451;

	// Min Integer value.
	static const int MinIntValue; //-2147483648;

	// Max Integer value.
	static const int MaxIntValue = 2147483647;

	// Min Int64 value.
	static const long long MinInt64Value; // = -9223372036854775808;

	// Max Int64 value.
	static const long long MaxInt64Value = 9223372036854775807;

	// Max UInt32 value.
	static const unsigned long MaxUInt32Value = 4294967295;

	// Max UInt64 value.
	static const UInt64 MaxUInt64Value = 18446744073709551615;

	// PI
	static const double PI;
		
}; // end class Constants


#endif