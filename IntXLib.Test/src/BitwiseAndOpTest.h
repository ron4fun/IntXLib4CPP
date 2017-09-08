#define BOOST_TEST_MODULE BitwiseAndOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BitwiseAndOpTest)

BOOST_AUTO_TEST_CASE(ShouldBitwiseAndTwoIntX)
{
	IntX int1 = IntX(11);
	IntX int2 = IntX(13);

	IntX result = int1 & int2;
	IntX result2 = int1 & 13;
	IntX result3 = (IntX)11 & int2;

	BOOST_CHECK(result == 9);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseAndPositiveAndNegativeIntX)
{
	IntX int1 = IntX(-11);
	IntX int2 = IntX(13);

	IntX result = int1 & int2;
	IntX result2 = int1 & 13;
	IntX result3 = (IntX)-11 & int2;

	BOOST_CHECK(result == 9);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseAndTwoNegativeIntX)
{
	IntX int1 = IntX(-11);
	IntX int2 = IntX(-13);

	IntX result = int1 & int2;
	IntX result2 = int1 & -13;
	IntX result3 = (IntX)-11 & int2;

	BOOST_CHECK(result == -9);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseAndIntXAndZero)
{
	IntX int1 = IntX(11);
	IntX int2 = IntX();

	IntX result = int1 & int2;
	IntX result2 = int1 & 0;
	IntX result3 = (IntX)11 & int2;

	BOOST_CHECK(result == 0);
	BOOST_CHECK(result == result2);
	BOOST_CHECK(result2 == result3);
}

BOOST_AUTO_TEST_CASE(ShouldBitwiseAndTwoBigIntX)
{
	IntX int1 = IntX(vector<UInt32>({ 11, 6, Constants::MaxUInt32Value, Constants::MaxUInt32Value }), false);
	IntX int2 = IntX(vector<UInt32>({ 13, 3, 0 }), false);

	IntX result = int1 & int2;

	BOOST_CHECK(result == IntX(vector<UInt32>({ 9, 2 }), false));
}

BOOST_AUTO_TEST_SUITE_END()
