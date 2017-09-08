#pragma once

#ifndef DIVIDERBASE_H
#define DIVIDERBASE_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IDivider.h"
#include "../Utils/Utils.h"
#include "../OpHelpers/DigitOpHelper.h"

#include <vector>

using namespace std;


/// <summary>
/// Base class for dividers.
/// Contains default implementation of divide operation over <see cref="IntX" /> instances.
/// </summary>
class DividerBase : public IDivider
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
	/// <exception cref="ArgumentNullException"><paramref name="int1" /> or <paramref name="int2" /> is a null reference.</exception>
	/// <exception cref="DivideByZeroException"><paramref name="int2" /> equals zero.</exception>
	virtual IntX DivMod(const IntX &int1, const IntX &int2, IntX &modRes, DivModResultFlags resultFlags)
	{
		// Check if int2 equals zero
		if (int2.length == 0)
		{
			throw DivideByZeroException("int2 is zero");
		} // end if

		// Get flags
		bool divNeeded = (resultFlags & DivModResultFlags::dmrfDiv) != 0;
		bool modNeeded = (resultFlags & DivModResultFlags::dmrfMod) != 0;

		// Special situation: check if int1 equals zero; in this case zero is always returned
		if (int1.length == 0)
		{
			modRes = modNeeded ? IntX() : IntX();
			return divNeeded ? IntX() : IntX();
		} // end if

		// Special situation: check if int2 equals one - nothing to divide in this case
		if (int2.length == 1 && int2.digits[0] == 1)
		{
			modRes = modNeeded ? IntX() : IntX();
			return divNeeded ? int2.negative ? (-(IntX(int1))) : int1 : IntX();
		} // end if

		// Get resulting sign
		bool resultNegative = int1.negative ^ int2.negative;

		// Check if int1 > int2
		int compareResult = DigitOpHelper::Cmp(&int1.digits[0], int1.length, &int2.digits[0], int2.length);
		if (compareResult < 0)
		{
			modRes = modNeeded ? IntX(int1) : IntX();
			return divNeeded ? IntX() : IntX();
		} // end if
		else if (compareResult == 0)
		{
			modRes = modNeeded ? IntX() : IntX();
			return divNeeded ? IntX(resultNegative ? -1 : 1) : IntX();
		} // end if

		//
		// Actually divide here (by Knuth algorithm)
		//

		// Prepare divident (if needed)
		IntX divRes = IntX();
		if (divNeeded)
		{
			divRes = IntX(int1.length - int2.length + 2U, resultNegative);
		} // end if

		// Prepare mod (if needed)
		if (modNeeded)
		{
			modRes = IntX(int1.length + 2U, int1.negative);
		} // end if
		else
		{
			modRes = IntX();
		} //end else

		// Call procedure itself
		UInt32 modLength = int1.length;
		UInt32 divLength = DivMod(
			(vector<UInt32>)int1.digits,
			modRes.digits, //modNeeded ? modRes.digits : vector<UInt32>(),
			modLength,
			(vector<UInt32>)int2.digits,
			vector<UInt32>(),
			int2.length,
			divRes.digits, //divNeeded ? divRes.digits : vector<UInt32>(),
			resultFlags,
			compareResult);

		// Maybe set new lengths and perform normalization
		if (divNeeded)
		{
			divRes.length = divLength;
			divRes.TryNormalize();
		} // end if
		if (modNeeded)
		{
			modRes.length = modLength;
			modRes.TryNormalize();
		} // end if

		// Return div
		return divRes;
	} // end function DivMod

	/// <summary>
	/// Divides two big integers.
	/// Also modifies <paramref name="digits1" /> and <paramref name="length1"/> (it will contain remainder).
	/// </summary>
	/// <param name="digits1">First big integer digits.</param>
	/// <param name="digitsBuffer1">Buffer for first big integer digits. May also contain remainder. Can be null - in this case it's created if necessary.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digits2">Second big integer digits.</param>
	/// <param name="digitsBuffer2">Buffer for second big integer digits. Only temporarily used. Can be null - in this case it's created if necessary.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsRes">Resulting big integer digits.</param>
	/// <param name="resultFlags">Which operation results to return.</param>
	/// <param name="cmpResult">Big integers comparsion result (pass -2 if omitted).</param>
	/// <returns>Resulting big integer length.</returns>
	virtual UInt32 DivMod(
		vector<UInt32> &digits1,
		vector<UInt32> &digitsBuffer1,
		UInt32 &length1,
		vector<UInt32> &digits2,
		vector<UInt32> &digitsBuffer2,
		UInt32 length2,
		vector<UInt32> &digitsRes,
		DivModResultFlags resultFlags,
		int cmpResult)
	{
		UInt32* digitsPtr1 = &digits1[0];
		UInt32* digitsBufferPtr1 = &digitsBuffer1[0];
		UInt32* digitsPtr2 = &digits2[0];
		UInt32* digitsBufferPtr2 = &digitsBuffer2[0];
		UInt32* digitsResPtr = &digitsRes[0];

		return DivMod(
				digitsPtr1,
				digitsBufferPtr1,
				length1,
				digitsPtr2,
				digitsBufferPtr2,
				length2,
				digitsResPtr,
				resultFlags,
				cmpResult);
	} // end function DivMod

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
		int cmpResult)
	{
		// Base implementation covers some special cases

		bool divNeeded = (resultFlags & DivModResultFlags::dmrfDiv) != 0;
		bool modNeeded = (resultFlags & DivModResultFlags::dmrfMod) != 0;

		//
		// Special cases
		//

		// Case when length1 == 0
		if (length1 == 0) return 0;

		// Case when both lengths are 1
		if (length1 == 1 && length2 == 1)
		{
			if (divNeeded)
			{
				*digitsResPtr = *digitsPtr1 / *digitsPtr2;
				if (*digitsResPtr == 0)
				{
					length2 = 0;
				} // end if
			} // end if
			if (modNeeded)
			{
				*digitsBufferPtr1 = *digitsPtr1 % *digitsPtr2;
				if (*digitsBufferPtr1 == 0)
				{
					length1 = 0;
				} // end if
			} // end if

			return length2;
		} // end if

		// Compare digits first (if was not previously compared)
		if (cmpResult == -2)
		{
			cmpResult = DigitOpHelper::Cmp(digitsPtr1, length1, digitsPtr2, length2);
		} // end if

		// Case when first value is smaller then the second one - we will have remainder only
		if (cmpResult < 0)
		{
			// Maybe we should copy first digits into remainder (if remainder is needed at all)
			if (modNeeded)
			{
				DigitHelper::DigitsBlockCopy(digitsPtr1, digitsBufferPtr1, length1);
			} // end if

			// Zero as division result
			return 0;
		} // end if

		// Case when values are equal
		if (cmpResult == 0)
		{
			// Maybe remainder must be marked as empty
			if (modNeeded)
			{
				length1 = 0;
			} // end if

			// One as division result
			if (divNeeded)
			{
				*digitsResPtr = 1;
			} // end if

			return 1;
		} // end if

		// Case when second length equals to 1
		if (length2 == 1)
		{
			// Call method basing on fact if div is needed
			UInt32 modRes;
			if (divNeeded)
			{
				length2 = DigitOpHelper::DivMod(digitsPtr1, length1, *digitsPtr2, digitsResPtr, modRes);
			} // end if
			else
			{
				modRes = DigitOpHelper::Mod(digitsPtr1, length1, *digitsPtr2);
			} // end else

			// Maybe save mod result
			if (modNeeded)
			{
				if (modRes != 0)
				{
					length1 = 1;
					*digitsBufferPtr1 = modRes;
				} // end if
				else
				{
					length1 = 0;
				} // end else
			} // end if

			return length2;
		} // end if


		// This is regular case, not special
		return Constants::MaxUInt32Value;
	} // end function DivMod

}; // end class DividerBase

#endif // !DIVIDERBASE_H

