#pragma once

#ifndef CLASSICDIVIDER_H
#define CLASSICDIVIDER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "DividerBase.h"
#include "../Utils/Constants.h"
#include "Bits.h"
#include "../OpHelpers/DigitHelper.h"
#include "../OpHelpers/DigitOpHelper.h"

// Divider using "classic" algorithm.
class ClassicDivider : public DividerBase
{
public:
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
		// Create some buffers if necessary
		if (digitsBuffer1.empty())
		{
			digitsBuffer1 = vector<UInt32>(length1 + 1);
		} // end if
		if (digitsBuffer2.empty())
		{
			digitsBuffer2 = vector<UInt32>(length2);
		} // end if

		UInt32* digitsPtr1 = &digits1[0], *digitsBufferPtr1 = &digitsBuffer1[0], *digitsPtr2 = &digits2[0], *digitsBufferPtr2 = &digitsBuffer2[0], *digitsResPtr = (!digitsRes.empty()) ? &digitsRes[0] : &digits1[0];
		
		return DivMod(
			digitsPtr1,
			digitsBufferPtr1,
			length1,
			digitsPtr2,
			digitsBufferPtr2,
			length2,
			digitsResPtr == digitsPtr1 ? nullptr : &digitsResPtr[0],
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

		bool divNeeded = (resultFlags & DivModResultFlags::dmrfDiv) != 0;
		bool modNeeded = (resultFlags & DivModResultFlags::dmrfMod) != 0;

		//
		// Prepare digitsBufferPtr1 and digitsBufferPtr2
		//

		int shift1 = 31 - Bits::Msb(digitsPtr2[length2 - 1]);
		if (shift1 == 0)
		{
			// We don't need to shift - just copy
			DigitHelper::DigitsBlockCopy(digitsPtr1, digitsBufferPtr1, length1);

			// We also don't need to shift second digits
			digitsBufferPtr2 = digitsPtr2;
		} // end if
		else
		{
			int rightShift1 = Constants::DigitBitCount - shift1;

			// We do need to shift here - so copy with shift - suppose we have enough storage for this operation
			length1 = DigitOpHelper::Shr(digitsPtr1, length1, (digitsBufferPtr1 + 1), rightShift1, true) + 1U;

			// Second digits also must be shifted
			DigitOpHelper::Shr(digitsPtr2, length2, (digitsBufferPtr2 + 1), rightShift1, true);
		} // end else

		//
		// Division main algorithm implementation
		//

		UInt64 longDigit;
		UInt64 divEst;
		UInt64 modEst;

		UInt64 mulRes;
		UInt32 divRes;
		long long k, t;

		// Some digits2 cached digits
		UInt32 lastDigit2 = digitsBufferPtr2[length2 - 1];
		UInt32 preLastDigit2 = digitsBufferPtr2[length2 - 2];

		// Main divide loop
		bool isMaxLength;
		UInt32 maxLength = length1 - length2;
		for (UInt32 i = maxLength, iLen2 = length1, j, ji; i <= maxLength; --i, --iLen2)
		{
			isMaxLength = iLen2 == length1;

			// Calculate estimates
			if (isMaxLength)
			{
				longDigit = digitsBufferPtr1[iLen2 - 1];
			} // end if
			else
			{
				longDigit = (UInt64)digitsBufferPtr1[iLen2] << Constants::DigitBitCount | digitsBufferPtr1[iLen2 - 1];
			} // end else

			divEst = longDigit / lastDigit2;
			modEst = longDigit - divEst * lastDigit2;

			// Check estimate (maybe correct it)
			for (;;)
			{
				if (divEst == Constants::BitCountStepOf2 || divEst * preLastDigit2 > (modEst << Constants::DigitBitCount) + digitsBufferPtr1[iLen2 - 2])
				{
					--divEst;
					modEst += lastDigit2;
					if (modEst < Constants::BitCountStepOf2) continue;
				} // end if
				break;
			} // end for
			divRes = (UInt32)divEst;

			// Multiply and subtract
			k = 0;
			for (j = 0, ji = i; j < length2; ++j, ++ji)
			{
				mulRes = (UInt64)divRes * digitsBufferPtr2[j];
				t = digitsBufferPtr1[ji] - k - (long long)(mulRes & 0xFFFFFFFF);
				digitsBufferPtr1[ji] = (UInt32)t;
				k = (long long)(mulRes >> Constants::DigitBitCount) - (long long)(t >> Constants::DigitBitCount);
			} // end for

			if (!isMaxLength)
			{
				t = digitsBufferPtr1[iLen2] - k;
				digitsBufferPtr1[iLen2] = (UInt32)t;
			} // end if
			else
			{
				t = -k;
			} // end else

			// Correct result if subtracted too much
			if (t < 0)
			{
				--divRes;

				k = 0;
				for (j = 0, ji = i; j < length2; ++j, ++ji)
				{
					t = (long long)digitsBufferPtr1[ji] + digitsBufferPtr2[j] + k;
					digitsBufferPtr1[ji] = (UInt32)t;
					k = (t >> Constants::DigitBitCount);
				} // end for

				if (!isMaxLength)
				{
					digitsBufferPtr1[iLen2] = (UInt32)(k + digitsBufferPtr1[iLen2]);
				} // end if
			} // end if

			// Maybe save div result
			if (divNeeded)
			{
				digitsResPtr[i] = divRes;
			} // end if
		} // end for

		if (modNeeded)
		{
			// First set correct mod length
			length1 = DigitHelper::GetRealDigitsLength(digitsBufferPtr1, length2);

			// Next maybe shift result back to the right
			if (shift1 != 0 && length1 != 0)
			{
				length1 = DigitOpHelper::Shr(digitsBufferPtr1, length1, digitsBufferPtr1, shift1, false);
			} // end if
		} // end if

		// Finally return length
		return !divNeeded ? 0 : (digitsResPtr[maxLength] == 0 ? maxLength : ++maxLength);
	} // end function DivMod

}; // end class ClassicDivider

#endif // !CLASSICDIVIDER_H

