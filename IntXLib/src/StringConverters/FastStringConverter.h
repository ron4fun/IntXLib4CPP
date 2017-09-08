#pragma once

#ifndef FASTSTRINGCONVERTER_H
#define FASTSTRINGCONVERTER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "StringConverterBase.h"
#include "IStringConverter.h"
#include "../Utils/Constants.h"
#include "Bits.h"
#include "../Dividers/IDivider.h"
#include "../Multipliers/IMultiplier.h"
#include "../Dividers/DivideManager.h"
#include "../Multipliers/MultiplyManager.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <stack>
#include "IntX.h"
#include "../OpHelpers/DigitHelper.h"

using namespace std;

// Fast ToString converting algorithm using divide-by-two.
class FastStringConverter : public StringConverterBase
{

private:
		IStringConverter *_classicStringConverter; // classic converter

public:
	
	/// <summary>
	/// Creates new <see cref="FastStringConverter" /> instance.
	/// </summary>
	/// <param name="pow2StringConverter">Converter for pow2 case.</param>
	/// <param name="classicStringConverter">Classic converter.</param>
	FastStringConverter(IStringConverter &pow2StringConverter, IStringConverter &classicStringConverter)
		: StringConverterBase(pow2StringConverter)
	{
		_classicStringConverter = &classicStringConverter;
	} // end cctor

	/// <summary>
	/// Converts digits from internal representaion into given base.
	/// </summary>
	/// <param name="digits">Big integer digits.</param>
	/// <param name="length">Big integer length.</param>
	/// <param name="numberBase">Base to use for output.</param>
	/// <param name="outputLength">Calculated output length (will be corrected inside).</param>
	/// <returns>Conversion result (later will be transformed to string).</returns>
	virtual vector<UInt32> ToStringOLd(const vector<UInt32> &digits, const UInt32 length, const UInt32 numberBase, UInt32 &outputLength)
	{
		vector<UInt32> outputArray = StringConverterBase::ToString(digits, length, numberBase, outputLength);

		// Maybe base method already converted this number
		if (!outputArray.empty()) return outputArray;

		// Check length - maybe use classic converter instead
		if (length < Constants::FastConvertLengthLowerBound || length > Constants::FastConvertLengthUpperBound)
		{
			return _classicStringConverter->ToString(digits, length, numberBase, outputLength);
		} // end if

		int resultLengthLog2 = Bits::CeilLog2(outputLength);
		UInt32 resultLength = 1U << resultLengthLog2;

		// Create and initially fill array for transofmed numbers storing
		vector<UInt32> resultArray(resultLength);
		//UInt32 *resultArray = new UInt32[resultLength];
		memset(&resultArray[0], 0, resultLength * sizeof(UInt32));
		memcpy(&resultArray[0], &digits[0], length * sizeof(UInt32));
		
		// Create and initially fill array with lengths
		vector<UInt32> resultArray2(resultLength);
		//UInt32 *resultArray2 = new UInt32[resultLength];
		memset(&resultArray2[0], 0, resultLength * sizeof(UInt32));

		resultArray2[0] = length;

		IMultiplier *multiplier = MultiplyManager::GetCurrentMultiplier();
		IDivider *divider = DivideManager::GetCurrentDivider();

		// Generate all needed pows of numberBase in stack
		stack<IntX> baseIntStack; // = new Stack(resultLengthLog2);
		
		IntX null = IntX();
		IntX baseInt = null;
		for (UInt32 i = 0; i < (UInt32)resultLengthLog2; ++i)
		{
			if (IntX::ReferenceEquals(baseInt, null))
				baseInt = numberBase;
			else
				baseInt = multiplier->Multiply(baseInt, baseInt);

			baseIntStack.push(baseInt); 
		} // end for

		// Create temporary buffer for second digits when doing div operation
		vector<UInt32> tempBuffer(baseInt.length);
		//UInt32 *tempBuffer = new UInt32[baseInt.length];
		memset(&tempBuffer[0], 0, baseInt.length * sizeof(UInt32));

		// We will use unsafe code here
		UInt32* const resultPtr1Const = &resultArray[0], * resultPtr2Const = &resultArray2[0], *tempBufferPtr = &tempBuffer[0];
		
		// Results pointers which will be modified (on swap)
		UInt32* resultPtr1 = resultPtr1Const;
		UInt32* resultPtr2 = resultPtr2Const;

		// Temporary variables used on swapping
		vector<UInt32> tempArray;
		//UInt32 *tempArray;
		//UInt32* tempPtr;

		// Variables used in cycle
		UInt32 *ptr1, *ptr2, *ptr1end;
		UInt32 loLength;

		// Outer cycle instead of recursion
		for (UInt32 innerStep = resultLength >> 1, outerStep = resultLength; innerStep > 0; innerStep >>= 1, outerStep >>= 1)
		{
			// Prepare pointers
			ptr1 = resultPtr1;
			ptr2 = resultPtr2;
			ptr1end = resultPtr1 + resultLength;

			// Get baseInt from stack and fix it too
			baseInt = (IntX)baseIntStack.top();
			baseIntStack.pop();
			
			UInt32* baseIntPtr = &baseInt.digits[0];
			
			// Cycle thru all digits and their lengths
			for (; ptr1 < ptr1end; ptr1 += outerStep, ptr2 += outerStep)
			{
				// Divide ptr1 (with length in *ptr2) by baseIntPtr here.
				// Results are stored in ptr2 & (ptr2 + innerStep), lengths - in *ptr1 and (*ptr1 + innerStep)
				loLength = *ptr2;
				*(ptr1 + innerStep) = divider->DivMod(
					ptr1,
					ptr2,
					loLength,
					baseIntPtr,
					tempBufferPtr,
					baseInt.length,
					ptr2 + innerStep,
					DivModResultFlags(DivModResultFlags::dmrfDiv | DivModResultFlags::dmrfMod),
					-2);
				*ptr1 = loLength;
			} // end for

			// After inner cycle resultArray will contain lengths and resultArray2 will contain actual values
			// so we need to swap them here
			tempArray = resultArray;
			resultArray = resultArray2;
			resultArray2 = tempArray;

			//tempPtr = resultPtr1;
			//resultPtr1 = resultPtr2;
			//resultPtr2 = tempPtr;
		} // end for

		// Retrieve real output length
		outputLength = DigitHelper::GetRealDigitsLength(&resultArray2[0], outputLength);

		// Create output array
		outputArray = vector<UInt32>(outputLength);

		// Copy each digit but only if length is not null
		UInt32* outputPtr = &outputArray[0];

		for (UInt32 i = 0; i < outputLength; ++i)
		{
			if (resultPtr2[i] != 0)
			{
				outputPtr[i] = resultPtr1[i];
			} // end if
		} // end for
	
		return outputArray;
	} // end function ToString

	virtual vector<UInt32> ToString(const vector<UInt32> &digits, const UInt32 length, const UInt32 numberBase, UInt32 &outputLength)
	{
		//UInt32 * const resultPtr1Const, * const resultPtr2Const, *tempBufferPtr;//
		UInt32 *resultPtr1, *resultPtr2, *ptr1, *ptr2, *ptr1end, *baseIntPtr, *outputPtr;

		vector<UInt32> outputArray = StringConverterBase::ToString(digits, length, numberBase, outputLength);

		// Maybe base method already converted this number
		if (!outputArray.empty())
			return outputArray;

		vector<UInt32> *resultArray, *resultArray2, tempBuffer;
		int resultLengthLog2, i;
		UInt32 resultLength, loLength, innerStep, outerStep, j;
		IMultiplier *multiplier;
		IDivider  *divider;
		stack<IntX> baseIntStack;
		IntX baseInt, null = IntX();

		// Check length - maybe use classic converter instead
		if ((length < Constants::FastConvertLengthLowerBound) ||
			(length > Constants::FastConvertLengthUpperBound))
			return _classicStringConverter->ToString(digits, length, numberBase, outputLength);

		resultLengthLog2 = Bits::CeilLog2(outputLength);
		resultLength = UInt32(1) << resultLengthLog2;

		// Create and initially fill array for transformed numbers storing
		resultArray = new vector<UInt32>(resultLength);
		//resultArray.resize(resultLength);
		memmove(&(*resultArray)[0], &digits[0], length * sizeof(UInt32));

		// Create and initially fill array with lengths
		resultArray2 = new vector<UInt32>(resultLength);
		//resultArray2.resize(resultLength);
		(*resultArray2)[0] = length;

		multiplier = MultiplyManager::GetCurrentMultiplier();
		divider = DivideManager::GetCurrentDivider();

		// Generate all needed pows of numberBase in stack

		baseInt = null;
		i = 0;
		while (i < resultLengthLog2)
		{
			if (IntX::ReferenceEquals(baseInt, null))
				baseInt = numberBase;
			else
				baseInt = multiplier->Multiply(baseInt, baseInt);

			baseIntStack.push(baseInt);
			++i;
		} // end while

		// Create temporary buffer for second digits when doing div operation
		tempBuffer.resize(baseInt.length);

		// We will use unsafe code here

		UInt32 * const resultPtr1Const = &(*resultArray)[0];
		UInt32 * const resultPtr2Const = &(*resultArray2)[0];
		UInt32 * const tempBufferPtr = &tempBuffer[0];

		// Results pointers which will be modified (on swap)
		resultPtr1 = resultPtr1Const;
		resultPtr2 = resultPtr2Const;

		// Outer cycle instead of recursion
		innerStep = resultLength >> 1;
		outerStep = resultLength;

		while (innerStep > 0)
		{
			ptr1 = resultPtr1;
			ptr2 = resultPtr2;

			ptr1end = resultPtr1 + resultLength;

			// Get baseInt from stack and fix it too
			baseInt = baseIntStack.top();
			baseIntStack.pop();

			baseIntPtr = &baseInt.digits[0];

			// Cycle thru all digits and their lengths

			while (ptr1 < ptr1end)
			{

				// Divide ptr1 (with length in *ptr2) by baseIntPtr here.
				// Results are stored in ptr2 & (ptr2 + innerStep), lengths - in *ptr1 and (*ptr1 + innerStep)

				loLength = *ptr2;
				*(ptr1 + innerStep) = divider->DivMod(ptr1, ptr2, loLength, baseIntPtr,
					tempBufferPtr, baseInt.length, (ptr2 + innerStep),
					DivModResultFlags(DivModResultFlags::dmrfDiv |	DivModResultFlags::dmrfMod), -2);

				*ptr1 = loLength;

				ptr1 = ptr1 + outerStep;
				ptr2 = ptr2 + outerStep;
			} // end while

			// After inner cycle resultArray will contain lengths and resultArray2 will contain actual values
			// so we need to swap them here
			::swap(resultArray, resultArray2);
			//vector<UInt32> *tempArray = resultArray;
			//*resultArray = *resultArray2;
			//*resultArray2 = *tempArray;

			UInt32 *tempPtr = resultPtr1;
			resultPtr1 = resultPtr2;
			resultPtr2 = tempPtr;

			innerStep = innerStep >> 1;
			outerStep = outerStep >> 1;
		} // end while

		// Retrieve real output length
		outputLength = DigitHelper::GetRealDigitsLength(&(*resultArray2)[0], outputLength);

		// Create output array
		outputArray.resize(outputLength);

		// Copy each digit but only if length is not null
		outputPtr = &outputArray[0];

		j = 0;
		while (j < outputLength)
		{
			if (resultPtr2[j] != 0)
				outputPtr[j] = resultPtr1[j];
			++j;
		} // end while
		
		delete resultArray;
		delete resultArray2;

		return outputArray;
	} // end function 

}; // end class FastStringConverter

#endif // !FASTSTRINGCONVERTER_H
