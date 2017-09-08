#define BOOST_TEST_MODULE ToStringFastTest

#include "../IntX.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ToStringFastTest)

const int StartLength = 1024;
const int LengthIncrement = 101;
const int RepeatCount = 10;

const int RandomStartLength = 1024;
const int RandomEndLength = 4000;
const int RandomRepeatCount = 10;

int _length = StartLength;

string GetAllNineChars(const UInt32 length)
{
	string temp;

	for (UInt32 i = 0; i < length; ++i)
		temp += '9';

	return temp;
} // end function GetAllNineChars

string GetRandomChars()
{
	UInt32 length = rand() % (RandomEndLength - RandomStartLength) + RandomStartLength; // randrange(RandomStartLength, RandomEndLength)

	string builder;

	builder += rand() % ('9' - '1') + '1';  // randrange('1', '9');
	--length;

	while (length-- != 0)
	{
		builder += rand() % ('9' - '0') + '0'; // randrange('0', '9'));
	} // end while

	return builder;
} // end function GetRandomChars

BOOST_AUTO_TEST_CASE(CompareWithClassic)
{
	srand(time(0));
	for (UInt32 i = 0; i < RepeatCount; ++i)
	{
		string str = GetAllNineChars(_length);
		IntX x = IntX::Parse(str, ParseMode::pmFast);

		x.getSettings().setToStringMode(ToStringMode::tsmFast);
		string strFast = x.ToString();

		x.getSettings().setToStringMode(ToStringMode::tsmClassic);
		string strClassic = x.ToString();

		BOOST_CHECK(str == strFast);
		BOOST_CHECK(strFast == strClassic);

		_length += LengthIncrement;
	} // end for
}

BOOST_AUTO_TEST_CASE(CompareWithClassicRandom)
{
	srand(time(0));
	for (UInt32 i = 0; i < RandomRepeatCount; ++i)
	{
		string str = GetRandomChars();
		IntX x = IntX::Parse(str, ParseMode::pmFast);

		x.getSettings().setToStringMode(ToStringMode::tsmFast);
		string strFast = x.ToString();

		x.getSettings().setToStringMode(ToStringMode::tsmClassic);
		string strClassic = x.ToString();

		BOOST_CHECK(str == strFast);
		BOOST_CHECK(strFast == strClassic);
	} // end for
}

BOOST_AUTO_TEST_SUITE_END()

