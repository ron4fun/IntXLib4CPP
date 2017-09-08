#pragma once

#ifndef IDIVIDER_H
#define IDIVIDER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "../IntX.h"

// Divider class interface.
class IDivider
{
public:
	
	/// <summary>
	/// Divides one <see cref="IntX" /> by another.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="modRes">Remainder big integer.</param>
	/// <param name="resultFlags">Which operation results to return.</param>
	/// <returns>Divident big integer.</returns>
	virtual IntX DivMod(const IntX &int1, const IntX &int2, IntX &modRes, DivModResultFlags resultFlags) = 0;
	
	/// <summary>
	/// Divides two big integers.
	/// Also modifies <paramref name="digitsPtr1" /> and <paramref name="length1"/> (it will contain remainder).
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="digitsBufferPtr1">Buffer for first big integer digits. May also contain remainder.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="digitsBufferPtr2">Buffer for second big integer digits. Only temporarily used.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <param name="resultFlags">Which operation results to return.</param>
	/// <param name="cmpResult">Big integers comparsion result (pass -2 if omitted).</param>
	/// <returns>Resulting big integer length.</returns>
	virtual UInt32 DivMod(
		UInt32* digitsPtr1,
		UInt32* digitsBufferPtr1,
		UInt32 &length1,
		UInt32* digitsPtr2,
		UInt32* digitsBufferPtr2,
		UInt32 length2,
		UInt32* digitsResPtr,
		DivModResultFlags resultFlags,
		int cmpResult) = 0;

	virtual UInt32 DivMod(
		vector<UInt32> &digits1,
		vector<UInt32> &digitsBuffer1,
		UInt32 &length1,
		vector<UInt32> &digits2,
		vector<UInt32> &digitsBuffer2,
		UInt32 length2,
		vector<UInt32> &digitsRes,
		DivModResultFlags resultFlags,
		int cmpResult) = 0;

}; // end class IDivider

#endif // !IDIVIDER_H

