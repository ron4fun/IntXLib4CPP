#pragma once

#ifndef AUTOFHTMULTIPLIER_H
#define AUTOFHTMULTIPLIER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "IMultiplier.h"
#include "MultiplierBase.h"
#include "../Utils/Constants.h"
#include "../OpHelpers/FhtHelper.h"
#include "../OpHelpers/DigitOpHelper.h"

#include <algorithm>
#include <vector>

using namespace std;

// Multiplies using FHT.
class AutoFhtMultiplier : public MultiplierBase
{
public:
	
	/// <summary>
	/// Creates new <see cref="AutoFhtMultiplier" /> instance.
	/// </summary>
	/// <param name="classicMultiplier">Multiplier to use if FHT is unapplicatible.</param>
	AutoFhtMultiplier(IMultiplier &classicMultiplier)
	{
		_classicMultiplier = &classicMultiplier;
	} // end .cctr

	/// <summary>
	/// Multiplies two big integers using pointers.
	/// </summary>
	/// <param name="digitsPtr1">First big integer digits.</param>
	/// <param name="length1">First big integer length.</param>
	/// <param name="digitsPtr2">Second big integer digits.</param>
	/// <param name="length2">Second big integer length.</param>
	/// <param name="digitsResPtr">Resulting big integer digits.</param>
	/// <returns>Resulting big integer real length.</returns>
	virtual UInt32 Multiply(const UInt32 *digitsPtr1, const UInt32 length1, const UInt32 *digitsPtr2,const UInt32 length2, UInt32 *digitsResPtr)
	{
		//unsigned int* digitsPtr1 = &vdigitsPtr1[0], *digitsPtr2 = &vdigitsPtr2[0], *digitsResPtr = &vdigitsResPtr[0];

		// Check length - maybe use classic multiplier instead
		if (length1 < Constants::AutoFhtLengthLowerBound || length2 < Constants::AutoFhtLengthLowerBound ||
			length1 > Constants::AutoFhtLengthUpperBound || length2 > Constants::AutoFhtLengthUpperBound)
		{
			return _classicMultiplier->Multiply(digitsPtr1, length1, digitsPtr2, length2, digitsResPtr);
		} // end if

		UInt32 newLength = length1 + length2;

		// Do FHT for first big integer
		vector<double> data1 = FhtHelper::ConvertDigitsToDouble(digitsPtr1, length1, newLength);
		FhtHelper::Fht(&data1[0], data1.size());

		// Compare digits
		vector<double> data2;
		if (digitsPtr1 == digitsPtr2 || DigitOpHelper::Cmp(digitsPtr1, length1, digitsPtr2, length2) == 0)
		{
			// Use the same FHT for equal big integers
			data2 = vector<double>(data1);
		} // end if
		else
		{
			// Do FHT over second digits
			data2 = FhtHelper::ConvertDigitsToDouble((UInt32 *)digitsPtr2, length2, newLength);
			FhtHelper::Fht(&data2[0], data2.size());
		} // end else

		// Perform multiplication and reverse FHT
		FhtHelper::MultiplyFhtResults(&data1[0], &data2[0], data1.size());
		FhtHelper::ReverseFht(&data1[0], data1.size());

		// Convert to digits
		double* slice1 = &data1[0];
		FhtHelper::ConvertDoubleToDigits(slice1, data1.size(), newLength, digitsResPtr);


		// Maybe check for validity using classic multiplication
		if (IntX::getGlobalSettings()->getApplyFhtValidityCheck())
		{
			UInt32 lowerDigitCount = min(length2, min(length1, Constants::FhtValidityCheckDigitCount));

			// Validate result by multiplying lowerDigitCount digits using classic algorithm and comparing
			vector<UInt32> validationResult(lowerDigitCount * 2);
			UInt32* validationResultPtr = &validationResult[0];
			
			_classicMultiplier->Multiply(digitsPtr1, lowerDigitCount, digitsPtr2, lowerDigitCount, &validationResult[0]);
			if (DigitOpHelper::Cmp(validationResultPtr, lowerDigitCount, digitsResPtr, lowerDigitCount) != 0)
			{
				throw FhtMultiplicationException(string(Strings::FhtMultiplicationError));
			} // end if
		} // end if
		
		//try
		//{
		//	digitsResPtr[newLength - 1];
		//} // end try
		//catch (const std::exception&e)
		//{
		//	--newLength;
		//} // end catch
		//UInt32 d = digitsResPtr[newLength - 2];
		return digitsResPtr[newLength - 1] == 0 ? --newLength : newLength;
		//return newLength;
	} // end function Multiply

private:
	IMultiplier *_classicMultiplier;

}; // end class AutoFhtMultiplier

#endif // !1
