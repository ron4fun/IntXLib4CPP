#define BOOST_TEST_MODULE DivOpNewtonTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(DivOpNewtonTest)

const int StartLength = 1024;
const int LengthIncrement = 101;
const int RepeatCount = 10;

const int RandomStartLength = 1024;
const int RandomEndLength = 2048;
const int RandomRepeatCount = 25;

int _length = StartLength;

void NextBytes(vector<unsigned char> &bytes)
{
	for (UInt32 i = 0; i < bytes.size(); ++i)
	{
		UInt32 randValue = rand() % 28 + 2; // randrange(2, 30)
		if ((randValue & 1) != 0)
			bytes[i] = unsigned char((randValue >> 1) ^ 0x25);
		else
			bytes[i] = unsigned char(randValue >> 1);
	} // end for
} // end function NextBytes

vector<UInt32> GetAllOneDigits(const UInt32 length)
{
	vector<UInt32> digits(length);
	for (UInt32 i = 0; i < digits.size(); ++i)
	{
		digits[i] = 0xFFFFFFFF;
	} // end for
	return digits;
} // end function GetAllOneDigits

vector<UInt32> GetRandomDigits(vector<UInt32> &digits2)
{
	vector<UInt32> digits(rand() % (RandomEndLength - RandomStartLength) + RandomStartLength);
	digits2 = vector<UInt32>(digits.size() / 2);

	vector<unsigned char> bytes(4);
	for (UInt32 i = 0; i < digits.size(); ++i)
	{
		NextBytes(bytes);
		digits[i] = bytes[0];
		if (i < digits2.size())
		{
			NextBytes(bytes);
			digits2[i] = bytes[0];
		} // end if
	} // end for

	return digits;
} // end function GetRandomDigits

BOOST_AUTO_TEST_CASE(CompareWithClassic)
{
	srand(time(0));
	for (UInt32 i = 0; i < RepeatCount; ++i)
	{
		IntX x = IntX(GetAllOneDigits(_length), true);
		IntX x2 = IntX(GetAllOneDigits(_length / 2), true);

		IntX classicMod, fastMod;
		IntX classic = IntX::DivideModulo(x, x2, classicMod, DivideMode::dmClassic);
		IntX fast = IntX::DivideModulo(x, x2, fastMod, DivideMode::dmAutoNewton);

		BOOST_CHECK(classic == fast);
		BOOST_CHECK(classicMod == fastMod);

		_length += LengthIncrement;
	} // end for
}

BOOST_AUTO_TEST_CASE(CompareWithClassicRandom)
{
	srand(time(0));
	for (UInt32 i = 0; i < RandomRepeatCount; ++i)
	{
		vector<UInt32> digits2;
		IntX x = IntX(GetRandomDigits(digits2), false);
		IntX x2 = IntX(digits2, false);

		IntX classicMod, fastMod;
		IntX classic = IntX::DivideModulo(x, x2, classicMod, DivideMode::dmClassic);
		IntX fast = IntX::DivideModulo(x, x2, fastMod, DivideMode::dmAutoNewton);

		BOOST_CHECK(classic == fast);
		BOOST_CHECK(classicMod == fastMod);
	} // end for
}

BOOST_AUTO_TEST_SUITE_END()
