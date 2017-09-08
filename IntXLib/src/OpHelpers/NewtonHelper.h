#pragma once

#ifndef NEWTONHELPER_H
#define NEWTONHELPER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include <vector>

#include "Bits.h"
#include "DigitOpHelper.h"
#include "../Utils/Constants.h"
#include "../Multipliers/IMultiplier.h"
#include "../Multipliers/MultiplyManager.h"

using namespace std;

// Contains helping methods for fast division
// using Newton approximation approach and fast multiplication.
class NewtonHelper
{
public:
	
	/// <summary>
	/// Generates integer opposite to the given one using approximation.
	/// Uses algorithm from Khuth vol. 2 3rd Edition (4.3.3).
	/// </summary>
	/// <param name="digitsPtr">Initial big integer digits.</param>
	/// <param name="length">Initial big integer length.</param>
	/// <param name="maxLength">Precision length.</param>
	/// <param name="bufferPtr">Buffer in which shifted big integer may be stored.</param>
	/// <param name="newLength">Resulting big integer length.</param>
	/// <param name="rightShift">How much resulting big integer is shifted to the left (or: must be shifted to the right).</param>
	/// <returns>Resulting big integer digits.</returns>
	static vector<UInt32> GetIntegerOpposite(
		const UInt32* digitsPtr,
		const UInt32 vlength,
		const UInt32 maxLength,
		UInt32* bufferPtr,
		UInt32 &newLength,
		UInt64 &rightShift)
	{
		UInt32 length = vlength;

		// Maybe initially shift original digits a bit to the left
		// (it must have MSB on 2nd position in the highest digit)
		int msb = Bits::Msb(digitsPtr[length - 1]);
		rightShift = (UInt64)(length - 1) * Constants::DigitBitCount + (UInt64)msb + 1U;

		if (msb != 2)
		{
			// Shift to the left (via actually right shift)
			int leftShift = (2 - msb + Constants::DigitBitCount) % Constants::DigitBitCount;
			length = DigitOpHelper::Shr(digitsPtr, length, bufferPtr + 1, Constants::DigitBitCount - leftShift, true) + 1U;
		} // end if
		else
		{
			// Simply use the same digits without any shifting
			bufferPtr = (UInt32*)digitsPtr;
		} // end else

		// Calculate possible result length
		int lengthLog2 = Bits::CeilLog2(maxLength);
		UInt32 newLengthMax = 1U << (lengthLog2 + 1);
		int lengthLog2Bits = lengthLog2 + Bits::Msb(Constants::DigitBitCount);

		// Create result digits
		vector<UInt32> resultDigits(newLengthMax);
		UInt32 resultLength;

		// Create temporary digits for squared result (twice more size)
		vector<UInt32> resultDigitsSqr(newLengthMax);
		UInt32 resultLengthSqr;

		// Create temporary digits for squared result * buffer
		vector<UInt32> resultDigitsSqrBuf(newLengthMax + length);
		UInt32 resultLengthSqrBuf;

		// We will always use current multiplier
		IMultiplier *multiplier = MultiplyManager::GetCurrentMultiplier();

		// Fix some digits
		UInt32* resultPtrFixed = &resultDigits[0], *resultSqrPtrFixed = &resultDigitsSqr[0], *resultSqrBufPtr = &resultDigitsSqrBuf[0];

		UInt32* resultPtr = resultPtrFixed;
		UInt32* resultSqrPtr = resultSqrPtrFixed;

		// Cache two first digits
		UInt32 bufferDigitN1 = bufferPtr[length - 1];
		UInt32 bufferDigitN2 = bufferPtr[length - 2];

		// Prepare result.
		// Initially result = floor(32 / (4*v1 + 2*v2 + v3)) / 4
		// (last division is not floored - here we emulate fixed point)
		resultDigits[0] = 32 / bufferDigitN1;
		resultLength = 1;

		// Prepare variables
		UInt32 nextBufferTempStorage = 0;
		int nextBufferTempShift;
		UInt32 nextBufferLength = 1U;
		UInt32* nextBufferPtr = &nextBufferTempStorage;

		UInt64 bitsAfterDotResult;
		UInt64 bitsAfterDotResultSqr;
		UInt64 bitsAfterDotNextBuffer;
		UInt64 bitShift;
		UInt32 shiftOffset;

		UInt32* tempPtr;
		//unsigned int* tempDigits;

		// Iterate 'till result will be precise enough
		for (int k = 0; k < lengthLog2Bits; ++k)
		{
			// Get result squared
			resultLengthSqr = multiplier->Multiply(
				resultPtr,
				resultLength,
				resultPtr,
				resultLength,
				resultSqrPtr);

			// Calculate current result bits after dot
			bitsAfterDotResult = (1UL << k) + 1UL;
			bitsAfterDotResultSqr = bitsAfterDotResult << 1;

			// Here we will get the next portion of data from bufferPtr
			if (k < 4)
			{
				// For now buffer intermediate has length 1 and we will use this fact
				nextBufferTempShift = 1 << (k + 1);
				nextBufferTempStorage =
					bufferDigitN1 << nextBufferTempShift |
					bufferDigitN2 >> (Constants::DigitBitCount - nextBufferTempShift);

				// Calculate amount of bits after dot (simple formula here)
				bitsAfterDotNextBuffer = (UInt64)nextBufferTempShift + 3UL;
			}
			else
			{
				// Determine length to get from bufferPtr
				nextBufferLength = min((1U << (k - 4)) + 1U, length);
				nextBufferPtr = bufferPtr + (length - nextBufferLength);

				// Calculate amount of bits after dot (simple formula here)
				bitsAfterDotNextBuffer = (UInt64)(nextBufferLength - 1U) * Constants::DigitBitCount + 3UL;
			}

			// Multiply result ^ 2 and nextBuffer + calculate new amount of bits after dot
			resultLengthSqrBuf = multiplier->Multiply(
				resultSqrPtr,
				resultLengthSqr,
				nextBufferPtr,
				nextBufferLength,
				resultSqrBufPtr);

			bitsAfterDotNextBuffer += bitsAfterDotResultSqr;

			// Now calculate 2 * result - resultSqrBufPtr
			--bitsAfterDotResult;
			--bitsAfterDotResultSqr;

			// Shift result on a needed amount of bits to the left
			bitShift = bitsAfterDotResultSqr - bitsAfterDotResult;
			shiftOffset = (UInt32)(bitShift / Constants::DigitBitCount);
			resultLength =
				shiftOffset + 1U +
				DigitOpHelper::Shr(
					resultPtr,
					resultLength,
					resultSqrPtr + shiftOffset + 1U,
					Constants::DigitBitCount - (int)(bitShift % Constants::DigitBitCount),
					true);

			// Swap resultPtr and resultSqrPtr pointers
			tempPtr = resultPtr;
			resultPtr = resultSqrPtr;
			resultSqrPtr = tempPtr;

			vector<UInt32> tempDigits = resultDigits;
			resultDigits = resultDigitsSqr;
			resultDigitsSqr = tempDigits;

			DigitHelper::SetBlockDigits(resultPtr, shiftOffset, 0U);

			bitShift = bitsAfterDotNextBuffer - bitsAfterDotResultSqr;
			shiftOffset = (UInt32)(bitShift / Constants::DigitBitCount);

			if (shiftOffset < resultLengthSqrBuf)
			{
				// Shift resultSqrBufPtr on a needed amount of bits to the right
				resultLengthSqrBuf = DigitOpHelper::Shr(
					resultSqrBufPtr + shiftOffset,
					resultLengthSqrBuf - shiftOffset,
					resultSqrBufPtr,
					(int)(bitShift % Constants::DigitBitCount),
					false);

				// Now perform actual subtraction
				resultLength = DigitOpHelper::Sub(
					resultPtr,
					resultLength,
					resultSqrBufPtr,
					resultLengthSqrBuf,
					resultPtr);
			} // end if
			else
			{
				// Actually we can assume resultSqrBufPtr == 0 here and have nothing to do
			} // end else
		} // end for

		rightShift += (1UL << lengthLog2Bits) + 1UL;
		newLength = resultLength;
		return resultDigits;
	} // end function

}; // end class NewtonHelper

#endif // !NEWTONHELPER_H
