#pragma once

#ifndef MULTIPLIERBASE_H
#define MULTIPLIERBASE_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IMultiplier.h"
#include "../Utils/Strings.h"
#include <string>
#include <vector>
#include "../IntX.h"
#include "../Utils/Utils.h"

using namespace std;


// Base class for multipliers.
// Contains default implementation of multiply operation over <see cref="IntX" /> instances.
class MultiplierBase : public IMultiplier
{
public:
	
	/// <summary>
	/// Multiplies two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="int1" /> or <paramref name="int2" /> is a null reference.</exception>
	/// <exception cref="ArgumentException"><paramref name="int1" /> or <paramref name="int2" /> is too big for multiply operation.</exception>
	virtual IntX Multiply(const IntX &int1, const IntX &int2)
	{
		// Special behavior for zero cases
		if (int1.length == 0 || int2.length == 0) return IntX();

		// Get new big integer length and check it
		UInt64 newLength = (UInt64)int1.length + (UInt64)int2.length;
		if (newLength >> 32 != 0)
		{
			throw ArgumentException(Strings::IntegerTooBig);
		} // end if

		// Create resulting big int
		IntX newInt = IntX((UInt32)newLength, int1.negative ^ int2.negative);

		// Perform actual digits multiplication
		newInt.length = Multiply(int1.digits, int1.length, int2.digits, int2.length, newInt.digits);

		// Normalization may be needed
		newInt.TryNormalize();
		
		return newInt;
	} // end function Multiply

	UInt32 Multiply(const vector<UInt32> &vdigitsPtr1, const UInt32 length1, const vector<UInt32> &vdigitsPtr2, const UInt32 length2, vector<UInt32> &vdigitsResPtr)
	{
		const UInt32* digitsPtr1 = &vdigitsPtr1[0], *digitsPtr2 = &vdigitsPtr2[0];
		UInt32 *digitsResPtr = &vdigitsResPtr[0];

		return Multiply(digitsPtr1, length1, digitsPtr2, length2, digitsResPtr);
	} // end function Multiply

	/// <summary>
	/// Multiplies two big integers using pointers.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <returns>Resulting big integer length.</returns>
	virtual UInt32 Multiply(const UInt32 *digitsPtr1, const UInt32 length1, const UInt32 *digitsPtr2, const UInt32 length2, UInt32 *digitsResPtr) = 0;
}; // end class MultiplierBase

#endif // !MULTIPLIERBASE_H
