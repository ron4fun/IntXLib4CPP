#define BOOST_TEST_MODULE MulOpFhtTest

#include "../IntX.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MulOpFhtTest)

const int StartLength = 256;
const int LengthIncrement = 101;
const int RepeatCount = 10;

const int RandomStartLength = 256;
const int RandomEndLength = 1000;
const int RandomRepeatCount = 50;

int _length = StartLength;

vector<UInt32> GetAllOneDigits(const UInt32 length)
{
	vector<UInt32> digits(length);
	for (UInt32 i = 0; i < digits.size(); ++i)
	{
		digits[i] = 0x7F7F7F7F;
	} // end for
	return digits;
} // end function GetAllOneDigits

vector<UInt32> GetRandomDigits(const UInt32 length)
{
	vector<UInt32> digits(length);
	for (UInt32 i = 0; i < digits.size(); ++i)
	{
		digits[i] = rand() * 2U;
	} // end for
	return digits;
} // end function GetRandomDigits

vector<UInt32> GetRandomDigits()
{
	return GetRandomDigits(rand() % (RandomEndLength - RandomStartLength) + RandomStartLength);
} // end function GetRandomDigits

BOOST_AUTO_TEST_CASE(CompareWithClassic)
{
	srand(time(0));
	for (UInt32 i = 0; i < RepeatCount; ++i)
	{
		IntX x = IntX(GetAllOneDigits(_length), true);
		IntX classic = IntX::Multiply(x, x, MultiplyMode::mmClassic);
		IntX fht = IntX::Multiply(x, x, MultiplyMode::mmAutoFht);

		BOOST_CHECK(classic == fht);

		_length += LengthIncrement;
	} // end for
}

BOOST_AUTO_TEST_CASE(SmallLargeCompareWithClassic)
{
	IntX x = IntX(GetAllOneDigits(50000), false);
	IntX y = IntX(GetAllOneDigits(512), false);
	IntX classic = IntX::Multiply(x, y, MultiplyMode::mmClassic);
	IntX fht = IntX::Multiply(x, y, MultiplyMode::mmAutoFht);

	BOOST_CHECK(classic == fht);
}

BOOST_AUTO_TEST_CASE(CompareWithClassicRandom)
{
	srand(time(0));
	for (UInt32 i = 0; i < RandomRepeatCount; ++i)
	{
		IntX x = IntX(GetRandomDigits(), false);
		IntX classic = IntX::Multiply(x, x, MultiplyMode::mmClassic);
		IntX fht = IntX::Multiply(x, x, MultiplyMode::mmAutoFht);

		BOOST_CHECK(classic == fht);
	} // end for
}

BOOST_AUTO_TEST_SUITE_END()
