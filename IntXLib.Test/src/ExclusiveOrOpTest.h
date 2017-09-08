#define BOOST_TEST_MODULE ExclusiveOrOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ExclusiveOrOpTest)

BOOST_AUTO_TEST_CASE(ShouldExclusiveOrTwoIntX)
{
	IntX int1 = IntX(3);
	IntX int2 = IntX(5);

	IntX result = int1 ^ int2;

	BOOST_CHECK(result == 6);
}

BOOST_AUTO_TEST_CASE(ShouldExclusiveOrPositiveAndNegativeIntX)
{
	IntX int1 = IntX(-3);
	IntX int2 = IntX(5);

	IntX result = int1 ^ int2;

	BOOST_CHECK(result == -6);
}

BOOST_AUTO_TEST_CASE(ShouldExclusiveOrTwoNegativeIntX)
{
	IntX int1 = IntX(-3);
	IntX int2 = IntX(-5);

	IntX result = int1 ^ int2;

	BOOST_CHECK(result == 6);
}

BOOST_AUTO_TEST_CASE(ShouldExclusiveOrIntXAndZero)
{
	IntX int1 = IntX(3);
	IntX int2 = IntX();

	IntX result = int1 ^ int2;

	BOOST_CHECK(result == int1);
}

BOOST_AUTO_TEST_CASE(ShouldExclusiveOrTwoBigIntX)
{
	IntX int1 = IntX(vector<UInt32>({ 3, 5, Constants::MaxUInt32Value }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 8,  Constants::MaxUInt32Value }), false);

	IntX result = int1 ^ int2;

	BOOST_CHECK(result == IntX(vector<UInt32>({ 2, 13 }), false));
}

BOOST_AUTO_TEST_CASE(ShouldExclusiveOrTwoBigIntXOfDifferentLength)
{
	IntX int1 = IntX(vector<UInt32>({ 3, 5, Constants::MaxUInt32Value, Constants::MaxUInt32Value }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 8,  Constants::MaxUInt32Value }), false);

	IntX result = int1 ^ int2;

	BOOST_CHECK(result == IntX(vector<UInt32>({ 2, 13, 0, Constants::MaxUInt32Value }), false));
}

BOOST_AUTO_TEST_SUITE_END()
