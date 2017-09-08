#pragma once

#ifndef IMULTIPLIER_H
#define IMULTIPLIER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <vector>
#include "../IntX.h"

using namespace std;

class IntX;

//	Multiplier class interface.
class IMultiplier
{
public:
	
	/// <summary>
	/// Multiplies two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	virtual IntX Multiply(const IntX &int1, const IntX &int2) = 0;

	/// <summary>
	/// Multiplies two big integers represented by their digits.
	/// </summary>
	/// <param name="digits1">First big integer digits.</param>
	/// <param name="length1">First big integer real length.</param>
	/// <param name="digits2">Second big integer digits.</param>
	/// <param name="length2">Second big integer real length.</param>
	/// <param name="digitsRes">Where to put resulting big integer.</param>
	/// <returns>Resulting big integer real length.</returns>
	virtual UInt32 Multiply(const UInt32 *digits1, const UInt32 length1, const UInt32 *digits2, const UInt32 length2, UInt32 *digitsRes) = 0;

}; // end class IMultiplier

#endif // !IMULTIPLIER_H
