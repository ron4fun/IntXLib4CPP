#pragma once

#ifndef AUTONEWTONDIVIDER_H
#define AUTONEWTONDIVIDER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "DividerBase.h"
#include "IDivider.h"
#include "../Utils/Constants.h"
#include "../OpHelpers/NewtonHelper.h"
#include "../Multipliers/IMultiplier.h"
#include "../Multipliers/MultiplyManager.h"
#include "../OpHelpers/DigitOpHelper.h"

#include <vector>

using namespace std;

// Divides using Newton approximation approach.
class AutoNewtonDivider : public DividerBase
{

private:
	IDivider *_classicDivider; // divider to use if Newton approach is unapplicatible

public:
	
	/// <summary>
	/// Creates new <see cref="AutoNewtonDivider" /> instance.
	/// </summary>
	/// <param name="classicDivider">Divider to use if Newton approach is unapplicatible.</param>
	AutoNewtonDivider(IDivider &classicDivider)
	{
		_classicDivider = &classicDivider;
	} // end cctor

private:
	
	/// <summary>
	/// Returns true if it's better to use classic algorithm for given big integers.
	/// </summary>
	/// <param name="length1">First big integer length.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <returns>True if classic algorithm is better.</returns>
	bool IsClassicAlgorithmNeeded(const UInt32 length1, const UInt32 length2)
	{
		return
			length1 < Constants::AutoNewtonLengthLowerBound || length2 < Constants::AutoNewtonLengthLowerBound ||
			length1 > Constants::AutoNewtonLengthUpperBound || length2 > Constants::AutoNewtonLengthUpperBound;
	} // end function IsClassicAlgorithmNeeded

public:
	
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
		// Maybe immediately use classic algorithm here
		if (IsClassicAlgorithmNeeded(length1, length2))
		{
			return _classicDivider->DivMod(
				digits1,
				digitsBuffer1,
				length1,
				digits2,
				digitsBuffer2,
				length2,
				digitsRes,
				resultFlags,
				cmpResult);
		} // end if

		// Create some buffers if necessary
		if (digitsBuffer1.empty())
		{
			digitsBuffer1 = vector<UInt32>(length1 + 1);
		} // end if

		UInt32* digitsPtr1 = &digits1[0], *digitsBufferPtr1 = &digitsBuffer1[0], *digitsPtr2 = &digits2[0], *digitsBufferPtr2 = (!digitsBuffer2.empty() ? &digitsBuffer2[0] : &digits1[0]), *digitsResPtr = (!digitsRes.empty() ? &digitsRes[0] : &digits1[0]);
		
		return DivMod(
			digitsPtr1,
			digitsBufferPtr1,
			length1,
			digitsPtr2,
			digitsBufferPtr2 == digitsPtr1 ? nullptr : digitsBufferPtr2,
			length2,
			digitsResPtr == digitsPtr1 ? nullptr : digitsResPtr,
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
		// Maybe immediately use classic algorithm here
		if (IsClassicAlgorithmNeeded(length1, length2))
		{
			return _classicDivider->DivMod(
				digitsPtr1,
				digitsBufferPtr1,
				length1,
				digitsPtr2,
				digitsBufferPtr2,
				length2,
				digitsResPtr,
				resultFlags,
				cmpResult);
		} // end if

		// Call base (for special cases)
		UInt32 resultLength = DividerBase::DivMod(
			digitsPtr1,
			digitsBufferPtr1,
			length1,
			digitsPtr2,
			digitsBufferPtr2,
			length2,
			digitsResPtr,
			resultFlags,
			cmpResult);
		if (resultLength != Constants::MaxUInt32Value) return resultLength;


		// First retrieve opposite for the divider
		UInt32 int2OppositeLength;
		UInt64 int2OppositeRightShift;
		vector<UInt32> int2OppositeDigits = NewtonHelper::GetIntegerOpposite(
			digitsPtr2,
			length2,
			length1,
			digitsBufferPtr1,
			int2OppositeLength,
			int2OppositeRightShift);

		// We will need to muptiply it by divident now to receive quotient.
		// Prepare digits for multiply result
		UInt32 quotLength;
		vector<UInt32> quotDigits(length1 + int2OppositeLength);

		IMultiplier *multiplier = MultiplyManager::GetCurrentMultiplier();

		// Fix some arrays
		UInt32* oppositePtr = &int2OppositeDigits[0], *quotPtr = &quotDigits[0];
		
		// Multiply
		quotLength = multiplier->Multiply(
			oppositePtr,
			int2OppositeLength,
			digitsPtr1,
			length1,
			quotPtr);

		// Calculate shift
		UInt32 shiftOffset = (UInt32)(int2OppositeRightShift / Constants::DigitBitCount);
		int shiftCount = (int)(int2OppositeRightShift % Constants::DigitBitCount);

		// Get the very first bit of the shifted part
		UInt32 highestLostBit;
		if (shiftCount == 0)
		{
			highestLostBit = quotPtr[shiftOffset - 1] >> 31;
		} // end if
		else
		{
			highestLostBit = quotPtr[shiftOffset] >> (shiftCount - 1) & 1U;
		} // end else

		// After this result must be shifted to the right - this is required
		quotLength = DigitOpHelper::Shr(
			quotPtr + shiftOffset,
			quotLength - shiftOffset,
			quotPtr,
			shiftCount,
			false);

		// Maybe quotient must be corrected
		if (highestLostBit == 1U)
		{
			quotLength = DigitOpHelper::Add(quotPtr, quotLength, &highestLostBit, 1U, quotPtr);
		} // end if

		// Check quotient - finally it might be too big.
		// For this we must multiply quotient by divider
		UInt32 quotDivLength;
		vector<UInt32> quotDivDigits(quotLength + length2);
		
		UInt32* quotDivPtr = &quotDivDigits[0];
		
		quotDivLength = multiplier->Multiply(quotPtr, quotLength, digitsPtr2, length2, quotDivPtr);

		int cmpRes = DigitOpHelper::Cmp(quotDivPtr, quotDivLength, digitsPtr1, length1);
		if (cmpRes > 0)
		{
			highestLostBit = 1;
			quotLength = DigitOpHelper::Sub(quotPtr, quotLength, &highestLostBit, 1U, quotPtr);
			quotDivLength = DigitOpHelper::Sub(quotDivPtr, quotDivLength, digitsPtr2, length2, quotDivPtr);
		} // end if

		// Now everything is ready and prepared to return results

		// First maybe fill remainder
		if ((resultFlags & DivModResultFlags::dmrfMod) != 0)
		{
			length1 = DigitOpHelper::Sub(digitsPtr1, length1, quotDivPtr, quotDivLength, digitsBufferPtr1);
		} // end if

		// And finally fill quotient
		if ((resultFlags & DivModResultFlags::dmrfDiv) != 0)
		{
			DigitHelper::DigitsBlockCopy(quotPtr, digitsResPtr, quotLength);
		} // end if
		else
		{
			quotLength = 0;
		} // end else

		return quotLength;
	} // end function  

}; // end class AutoNewtonDivider

#endif // !AUTONEWTONDIVIDER_H

