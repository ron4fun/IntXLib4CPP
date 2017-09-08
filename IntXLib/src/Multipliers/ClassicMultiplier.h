#pragma once

#ifndef CLASSICMULTIPLIER_H
#define CLASSICMULTIPLIER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IMultiplier.h"
#include "MultiplierBase.h"
#include "../OpHelpers/DigitHelper.h"
#include <vector>

using namespace std;

// Multiplies using "classic" algorithm.
class ClassicMultiplier : public MultiplierBase
{
public:
	
	/// <summary>
	/// Multiplies two big integers using pointers.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>	
	virtual UInt32 Multiply(const UInt32 *digitsPtr1, const UInt32 vlength1, const UInt32 *digitsPtr2, const UInt32 vlength2, UInt32 *digitsResPtr)
	{
		UInt32 length1 = vlength1;
		UInt32 length2 = vlength2;
		UInt64 c;

		// External cycle must be always smaller
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

		// Prepare end pointers
		const UInt32* digitsPtr1End = digitsPtr1 + length1;
		const UInt32* digitsPtr2End = digitsPtr2 + length2;

		// We must always clear first "length1" digits in result
		DigitHelper::SetBlockDigits(digitsResPtr, length1, 0U);

		// Perform digits multiplication
		UInt32* ptr1 = nullptr, *ptrRes = nullptr;
		for (; digitsPtr2 < digitsPtr2End; ++digitsPtr2, ++digitsResPtr)
		{
			// Check for zero (sometimes may help). There is no sense to make this check in internal cycle -
			// it would give performance gain only here
			if (*digitsPtr2 == 0) continue;

			c = 0;
			for (ptr1 = (UInt32*)digitsPtr1, ptrRes = digitsResPtr; ptr1 < digitsPtr1End; ++ptr1, ++ptrRes)
			{
				c += (UInt64)*digitsPtr2 * *ptr1 + *ptrRes;
				*ptrRes = (UInt32)c;
				c >>= 32;
			} // end for
			*ptrRes = (UInt32)c;
		} // end for

		UInt32 newLength = length1 + length2;
		if (newLength > 0 && (ptrRes == nullptr || *ptrRes == 0))
		{
			--newLength;
		} // end if

		return newLength;
	} // end function Multiply

}; // end class ClassicMultiplier

#endif // !CLASSICMULTIPLIER_H
