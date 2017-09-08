#pragma once

#ifndef DIGITOPHELPER_H
#define DIGITOPHELPER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "../Utils/Constants.h"
#include "DigitHelper.h"
#include "../Utils/Utils.h"

class DigitOpHelper
{
public:
	
	/// <summary>
	/// Adds two big integers using pointers.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>
	static UInt32 Add(const UInt32* digitsPtr1, const UInt32 vlength1,
		const UInt32* digitsPtr2, const UInt32 vlength2, UInt32* digitsResPtr)
	{
		UInt32 length1 = vlength1;
		UInt32 length2 = vlength2;
		UInt64 c = 0;

		if (length1 < length2)
		{
			// First must be bigger - swap
			UInt32 lengthTemp = length1;
			length1 = length2;
			length2 = lengthTemp;

			UInt32* ptrTemp = (UInt32*)digitsPtr1;
			digitsPtr1 = digitsPtr2;
			digitsPtr2 = ptrTemp;
		} // end if

		// Perform digits adding
		for (UInt32 i = 0; i < length2; ++i)
		{
			c += (UInt64)(digitsPtr1[i]) + (UInt64)digitsPtr2[i];
			digitsResPtr[i] = (UInt32)c;
			c >>= 32;
		} // end for

		// Perform digits + carry moving
		for (UInt32 i = length2; i < length1; ++i)
		{
			c += digitsPtr1[i];
			digitsResPtr[i] = (UInt32)c;
			c >>= 32;
		} // end for

		// Account last carry
		if (c != 0)
		{
			digitsResPtr[length1++] = (UInt32)c;
		} // end if

		return length1;
	} // end function Add

	/// <summary>
	/// Subtracts two big integers using pointers.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>
	static UInt32 Sub(const UInt32* digitsPtr1, const UInt32 vlength1,
		const UInt32* digitsPtr2, const UInt32 vlength2, UInt32* digitsResPtr)
	{
		UInt32 length1 = vlength1;
		UInt32 length2 = vlength2;
		UInt64 c = 0;

		// Perform digits subtraction
		for (UInt32 i = 0; i < length2; ++i)
		{
			c = (UInt64)digitsPtr1[i] - (UInt64)digitsPtr2[i] - c;
			digitsResPtr[i] = (UInt32)c;
			c >>= 63;
		} // end for

		// Perform digits + carry moving
		for (UInt32 i = length2; i < length1; ++i)
		{
			c = digitsPtr1[i] - c;
			digitsResPtr[i] = (UInt32)c;
			c >>= 63;
		} // end for

		return DigitHelper::GetRealDigitsLength(digitsResPtr, length1);
	} // end function Sub

	/// <summary>
	/// Divides one big integer represented by it's digits on another one big ingeter.
	/// Reminder is always filled (but not the result).
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="int2">Second integer.</param>
	/// <param name="divResPtr">Div result (can be null - not filled in this case).</param>
	/// <param name="modRes">Remainder (always filled).</param>
	/// <returns>Result length (0 if result is null).</returns>
	static UInt32 DivMod(const UInt32* digitsPtr1, const UInt32 vlength1,
		const UInt32 int2, UInt32* divResPtr, UInt32 &modRes)
	{
		UInt32 length1 = vlength1;
		UInt64 c = 0;
		UInt32 res;
		for (UInt32 index = length1 - 1; index < length1; --index)
		{
			c = (c << Constants::DigitBitCount) + digitsPtr1[index];
			res = (UInt32)(c / int2);
			c -= (UInt64)(res) * int2;

			divResPtr[index] = res;
		} // end for
		modRes = (UInt32)c;

		return length1 - (divResPtr[length1 - 1] == 0 ? 1U : 0U);
	} // end function DivMod

	/// <summary>
	/// Divides one big integer represented by it's digits on another one big ingeter.
	/// Only remainder is filled.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="int2">Second integer.</param>
	/// <returns>Remainder.</returns>
	static UInt32 Mod(const UInt32* digitsPtr1, const UInt32 vlength1, const UInt32 int2)
	{
		UInt32 length1 = vlength1;
		UInt64 c = 0;
		UInt32 res;
		for (UInt32* ptr1 = (UInt32*)digitsPtr1 + length1 - 1; ptr1 >= digitsPtr1; --ptr1)
		{
			c = (c << Constants::DigitBitCount) + *ptr1;
			res = (UInt32)(c / int2);
			c -= (UInt64)(res) * int2;
		} // end for

		return (UInt32)c;
	} // end function Mod
		
	/// <summary>
	/// Compares 2 <see cref="IntX" /> objects represented by pointers only (not taking sign into account).
	/// Returns "-1" if <paramref name="digitsPtr1" /> &lt; <paramref name="digitsPtr2" />, "0" if equal and "1" if &gt;.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <returns>Comparsion result.</returns>
	static int Cmp(const UInt32* digitsPtr1, const UInt32 vlength1,
		const UInt32* digitsPtr2, const UInt32 vlength2)
	{
		UInt32 length1 = vlength1;
		UInt32 length2 = vlength2;

		// Maybe length comparing will be enough
		int res = CmpLen(length1, length2);
		if (res != -2) return res;

		for (UInt32 index = length1 - 1; index < length1; --index)
		{
			if (digitsPtr1[index] != digitsPtr2[index]) return digitsPtr1[index] < digitsPtr2[index] ? -1 : 1;
		} // end for
		return 0;
	} // end function Cmp

	/// <summary>
	/// Compares two integers lengths. Returns -2 if further comparing is needed.
	/// </summary>
	/// <param name="length1">First big integer length.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <returns>Comparsion result.</returns>
	static int CmpLen(const UInt32 length1, const UInt32 length2)
	{
		if (length1 < length2) return -1;
		if (length1 > length2) return 1;
		return length1 == 0 ? 0 : -2;
	} // end function CmpLen

	/// <summary>
	/// Shifts big integer.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="offset">Big integer digits offset.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="digitsRes">Resulting big integer digits.</param>
	/// <param name="resOffset">Resulting big integer digits offset.</param>
	/// <param name="rightShift">Shift to the right (always between 1 an 31).</param>
	static void Shr(
		const UInt32 digits[],
		const UInt32 offset,
		const UInt32 length,
		UInt32 digitsRes[],
		UInt32 resOffset,
		int rightShift)
	{
		UInt32 *digitsPtr = (UInt32 *)digits;
		UInt32 *digitsResPtr = digitsRes;
			
		Shr(digitsPtr + offset, length, digitsResPtr + resOffset, rightShift, resOffset != 0);
	} //end function Shr

	static UInt32 CypherShr(const UInt32* digitsPtr, const UInt32 vlength,
		UInt32* digitsResPtr, const int rightShift, const bool resHasOffset)
	{
		int rightShiftRev;
		const UInt32 *digitsPtrEndPrev, *digitsPtrNext;
		UInt32 lastValue, length = vlength;

		rightShiftRev = Constants::DigitBitCount - rightShift;

		// Shift first digit in special way
		if (resHasOffset)
			*(digitsResPtr - 1) = digitsPtr[0] << rightShiftRev;

		if (rightShift == 0)
		{
			// Handle special situation here - only memcpy is needed (maybe)
			if (digitsPtr != digitsResPtr)
				DigitHelper::DigitsBlockCopy(digitsPtr, digitsResPtr, length);
		} // end if
		else
		{

			// Shift all digits except last one
			digitsPtrEndPrev = digitsPtr + length - 1;
			digitsPtrNext = digitsPtr + 1;

			while (digitsPtr < digitsPtrEndPrev)
			{

				*digitsResPtr = *digitsPtr >> rightShift | *digitsPtrNext << rightShiftRev;
				++digitsPtr;
				++digitsPtrNext;
				++digitsResPtr;
			} // end while

			// Shift last digit in special way
			lastValue = *digitsPtr >> rightShift;
			if (lastValue != 0)
				*digitsResPtr = lastValue;
			else
				--length;
		} // end else

		return length;
	} // end function 

	/// <summary>
	/// Shifts big integer.
	/// </summary>
	/// <param name="digitsPtr">Big integer digits.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <param name="rightShift">Shift to the right (always between 1 an 31).</param>
	/// <param name="resHasOffset">True if <paramref name="digitsResPtr" /> has offset.</param>
	/// <returns>Resulting big integer length.</returns>
	static unsigned int Shr(const UInt32* digitsPtr, const UInt32 vlength,
		UInt32* digitsResPtr, const int rightShift, const bool resHasOffset)
	{
		UInt32 length = vlength;

		int rightShiftRev = Constants::DigitBitCount - rightShift;

		// Shift first digit in special way
		if (resHasOffset)
		{
			*(digitsResPtr - 1) = digitsPtr[0] << rightShiftRev;
		} // end for

		if (rightShift == 0)
		{
			// Handle special situation here - only memcpy is needed (maybe)
			if (digitsPtr != digitsResPtr)
			{
				DigitHelper::DigitsBlockCopy(digitsPtr, digitsResPtr, length);
			} // end if
		} // end if
		else
		{
			// Shift all digits except last one
			UInt32* digitsPtrEndPrev = (UInt32 *)digitsPtr + length - 1;
			UInt32* digitsPtrNext = (UInt32 *)digitsPtr + 1;
			for (; digitsPtr < digitsPtrEndPrev; ++digitsPtr, ++digitsPtrNext, ++digitsResPtr)
			{
				*digitsResPtr = (*digitsPtr >> rightShift) | (*digitsPtrNext << rightShiftRev);		
			}

			// Shift last digit in special way
			UInt32 lastValue = *digitsPtr >> rightShift;
   			if (lastValue != 0)
			{
				*digitsResPtr = lastValue;
			} // end if
			else
			{
				--length;
			} // end else
		} // end else

		return length;
	} // end function Shr

	/// <summary>
	/// Performs bitwise OR for two big integers.
	/// </summary>
	/// <param name="digits1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digits2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsRes">Resulting big integer digits.</param>
	static void BitwiseOr(const UInt32 digits1[], const UInt32 length1,
		const UInt32 digits2[], const UInt32 length2, UInt32 digitsRes[])
	{
		UInt32* digitsPtr1 = (UInt32*)digits1;
		UInt32* digitsPtr2 = (UInt32*)digits2;
		UInt32* digitsResPtr = digitsRes;
		
		for (UInt32 i = 0; i < length2; i++)
		{
			digitsResPtr[i] = digitsPtr1[i] | digitsPtr2[i];
		} // end for

		DigitHelper::DigitsBlockCopy(digitsPtr1 + length2, digitsResPtr + length2, length1 - length2);
	} // end function BitwiseOr

	/// <summary>
	/// Performs bitwise AND for two big integers.
	/// </summary>
	/// <param name="digits1">First big integer digits.</param>
	/// <param name="digits2">Second big integer digits.</param>
	/// <param name="length">Shorter big integer length.</param>
	/// <param name="digitsRes">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>
	static UInt32 BitwiseAnd(const UInt32 digits1[], const UInt32 digits2[],
		const UInt32 length, UInt32 digitsRes[])
	{
		UInt32* digitsPtr1 = (UInt32*)digits1;
		UInt32* digitsPtr2 = (UInt32*)digits2;
		UInt32* digitsResPtr = digitsRes;
		
		for (UInt32 i = 0; i < length; i++)
		{
			digitsResPtr[i] = digitsPtr1[i] & digitsPtr2[i];
		} // end for

		return DigitHelper::GetRealDigitsLength(digitsResPtr, length);
	}  // end function BitwiseAnd

	/// <summary>
	/// Performs bitwise XOR for two big integers.
	/// </summary>
	/// <param name="digits1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digits2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsRes">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>
	static UInt32 ExclusiveOr(const UInt32 digits1[], const UInt32 length1,
		const UInt32 digits2[], const UInt32 length2, UInt32 digitsRes[])
	{
		UInt32* digitsPtr1 = (UInt32*)digits1;
		UInt32* digitsPtr2 = (UInt32*)digits2;
		UInt32* digitsResPtr = digitsRes;
		
		for (UInt32 i = 0; i < length2; i++)
		{
			digitsResPtr[i] = digitsPtr1[i] ^ digitsPtr2[i];
		} // end for

		DigitHelper::DigitsBlockCopy(digitsPtr1 + length2, digitsResPtr + length2, length1 - length2);

		return DigitHelper::GetRealDigitsLength(digitsResPtr, length1);
	} // end function ExclusiveOr

	/// <summary>
	/// Performs bitwise NOT for big integer.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="digitsRes">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>
	static UInt32 OnesComplement(const UInt32 digits[], const UInt32 length, UInt32 digitsRes[])
	{
		UInt32* digitsPtr = (UInt32 *)digits;
		UInt32* digitsResPtr = digitsRes;
		
		for (UInt32 i = 0; i < length; i++)
		{
			digitsResPtr[i] = ~digitsPtr[i];
		} // end for

		return DigitHelper::GetRealDigitsLength(digitsResPtr, length);
	} // end function OnesComplement

}; // end class DigitOpHelper


#endif //!DIGITOPHELPER_H