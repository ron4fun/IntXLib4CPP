#pragma once

#ifndef DIGITHELPER_H
#define DIGITHELPER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "../Utils/Constants.h"


class DigitHelper
{
public:
	
	/// <summary>
	/// Returns real length of digits array (excluding leading zeroes).
	/// </summary>
	/// <param name="digits">Big ingeter digits.</param>
	/// <param name="length">Initial big integers length.</param>
	/// <returns>Real length.</returns>
	static UInt32 GetRealDigitsLength(const UInt32* digits, const UInt32 vlength)
	{
		UInt32 length = vlength;
		for (; length > 0 && digits[length - 1] == 0; --length);
		return length;
	} // end function GetRealDigitsLength
		
	/// <summary>
	/// Converts int value to uint digit and value sign.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <param name="resultValue">Resulting unsigned part.</param>
	/// <param name="negative">Resulting sign.</param>
	static void ToUInt32WithSign(const int value, UInt32 &resultValue, bool &negative)
	{
		negative = value < 0;
		resultValue = !negative
			? (UInt32)value
			: value != Constants::MinIntValue ? (UInt32)-value : Constants::MaxIntValue + 1U;
	} // end function ToUInt32WithSign

	/// <summary>
	/// Converts long value to ulong digit and value sign.
	/// </summary>
	/// <param name="value">Initial value.</param>
	/// <param name="resultValue">Resulting unsigned part.</param>
	/// <param name="negative">Resulting sign.</param>
	static void ToUInt64WithSign(const long long value, UInt64 &resultValue, bool &negative)
	{
		negative = value < 0;
		resultValue = !negative
			? (UInt64)value
			: value != Constants::MinInt64Value ? (UInt64)-value : (UInt64)(Constants::MaxInt64Value) + 1UL;
	} // end function ToUInt64WithSign
		
	/// <summary>
	/// Sets digits in given block to given value.
	/// </summary>
	/// <param name="block">Block start pointer.</param>
	/// <param name="blockLength">Block length.</param>
	/// <param name="value">Value to set.</param>
	static void SetBlockDigits(UInt32* block, const UInt32 blockLength, const UInt32 value)
	{
		for (UInt32* blockEnd = block + blockLength; block < blockEnd; *block++ = value);
	} // end function SetBlockDigits

	/// <summary>
	/// Sets digits in given block to given value.
	/// </summary>
	/// <param name="block">Block start pointer.</param>
	/// <param name="blockLength">Block length.</param>
	/// <param name="value">Value to set.</param>
	static void SetBlockDigits(double* block, const UInt32 blockLength, const double value)
	{
		for (double* blockEnd = block + blockLength; block < blockEnd; *block++ = value);
	} // end function SetBlockDigits

	/// <summary>
	/// Copies digits from one block to another.
	/// </summary>
	/// <param name="blockFrom">From block start pointer.</param>
	/// <param name="blockTo">To block start pointer.</param>
	/// <param name="count">Count of dwords to copy.</param>
	static void DigitsBlockCopy(const UInt32*  vblockFrom, UInt32* blockTo, const UInt32 count)
	{
		UInt32 * blockFrom = (UInt32*)vblockFrom;
		for (UInt32* blockFromEnd = blockFrom + count; blockFrom < blockFromEnd; *blockTo++ = *blockFrom++);
	} // end function DigitsBlockCopy

}; // end class DigitHelper

#endif //!DIGITHELPER_H