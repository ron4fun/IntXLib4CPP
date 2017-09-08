#define BOOST_TEST_MODULE ParseFastTest

#include "../IntX.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ParseFastTest)

const int StartLength = 1024;
const int LengthIncrement = 101;
const int RepeatCount = 50;

const int RandomStartLength = 1024;
const int RandomEndLength = 4000;
const int RandomRepeatCount = 50;

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
		IntX classic = IntX::Parse(str, ParseMode::pmClassic);
		IntX fast = IntX::Parse(str, ParseMode::pmFast);

		BOOST_CHECK(classic == fast);

		_length += LengthIncrement;
	} // end for
}

BOOST_AUTO_TEST_CASE(CompareWithClassicRandom)
{
	srand(time(0));
	for (UInt32 i = 0; i < RandomRepeatCount; ++i)
	{
		string str = GetRandomChars();
		IntX classic = IntX::Parse(str, ParseMode::pmClassic);
		IntX fast = IntX::Parse(str, ParseMode::pmFast);

		BOOST_CHECK(classic == fast);
	} // end for
}

BOOST_AUTO_TEST_SUITE_END()
