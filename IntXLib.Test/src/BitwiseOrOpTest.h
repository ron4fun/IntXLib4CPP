#define BOOST_TEST_MODULE BitwiseOrOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BitwiseOrOpTest)

BOOST_AUTO_TEST_CASE(ShouldBitwiseOrTwoIntX)
{
	IntX int1 = IntX(3);
	IntX int2 = IntX(5);

	IntX result = int1 | int2;
	IntX result2 = int1 | 5;
	IntX result3 = (IntX)3 | int2;

	BOOST_CHECK(result == 7);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseOrPositiveAndNegativeIntX)
{
	IntX int1 = IntX(-3);
	IntX int2 = IntX(5);

	IntX result = int1 | int2;
	IntX result2 = int1 | 5;
	IntX result3 = (IntX)-3 | int2;

	BOOST_CHECK(result == -7);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseOrTwoNegativeIntX)
{
	IntX int1 = IntX(-3);
	IntX int2 = IntX(-5);

	IntX result = int1 | int2;
	IntX result2 = int1 | -5;
	IntX result3 = (IntX)-3 | int2;

	BOOST_CHECK(result == -7);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseOrIntXAndZero)
{
	IntX int1 = IntX(3);
	IntX int2 = IntX();

	IntX result = int1 | int2;
	IntX result2 = int1 | 0;
	IntX result3 = (IntX)3 | int2;

	BOOST_CHECK(result == int1);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseOrTwoBigIntX)
{
	IntX int1 = IntX(vector<UInt32>({ 3, 5, Constants::MaxUInt32Value }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 8 }), false);

	IntX result = int1 | int2;

	BOOST_CHECK(result == IntX(vector<UInt32>({ 3, 13, Constants::MaxUInt32Value }), false));
}

BOOST_AUTO_TEST_SUITE_END()
