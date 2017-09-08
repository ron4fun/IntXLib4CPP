#pragma once

#ifndef DIGITCONVERTER_H
#define DIGITCONVERTER_H

#include <vector>
#include <stdlib.h>
#include <string>
#include "Strings.h"
#include "Utils.h"

using namespace std;

// Converts <see cref="IntX"/> digits to/from byte array.
class DigitConverter
{
public:
	/*
	/// <summary>
	/// Converts big integer digits to bytes.
	/// </summary>
	/// <param name="digits"><see cref="IntX" /> digits.</param>
	/// <returns>Resulting bytes.</returns>
	/// <remarks>
	/// Digits can be obtained using <see cref="IntX.GetInternalState" /> method.
	/// </remarks>
	*/
	static vector<unsigned char> ToBytes(const vector<unsigned int> &digits)
	{
		if (digits.empty())
		{
			throw ArgumentNullException("digits");
		} // end if

		vector<unsigned char> bytes(digits.size() * 4);
		memcpy(&bytes, &digits, bytes.size());

		return bytes;
	} // end function ToBytes

	/*
	/// <summary>
	/// Converts bytes to big integer digits.
	/// </summary>
	/// <param name="bytes">Bytes.</param>
	/// <returns>Resulting <see cref="IntX" /> digits.</returns>
	/// <remarks>
	/// Big integer can be created from digits using <see cref="IntX(uint[], bool)" /> constructor.
	/// </remarks>
	*/
	static vector<unsigned int> FromBytes(const vector<unsigned char> &bytes)
	{
		if (bytes.empty())
		{
			throw ArgumentNullException("bytes");
		} // end if
		if (bytes.size() % 4 != 0)
		{
			throw ArgumentException(string(Strings::DigitBytesLengthInvalid) + "bytes");
		} // end if

		vector<unsigned int> digits(bytes.size() / 4);
		memcpy(&digits, &bytes, bytes.size());
		
		return digits;
	} // end function FromBytes

}; // end class DigitConverter

#endif // !DIGITCONVERTER_H
