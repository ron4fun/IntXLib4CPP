#define BOOST_TEST_MODULE SubOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(SubOpTest)

BOOST_AUTO_TEST_CASE(Sub2IntX)
{
	IntX int1 = IntX(3);
	IntX int2 = IntX(5);
	BOOST_CHECK(int1 - int2 == -2);
	BOOST_CHECK((IntX)3 - int2 == -2);
	BOOST_CHECK(int1 - 5 == -2);
}

BOOST_AUTO_TEST_CASE(Sub2IntXNeg)
{
	IntX int1 = IntX(-3);
	IntX int2 = IntX(-5);
	BOOST_CHECK(int1 - int2 == 2);
	BOOST_CHECK((IntX)-3 - int2 == 2);
	BOOST_CHECK(int1 - (-5) == 2);
}

BOOST_AUTO_TEST_CASE(SubIntIntX)
{
	IntX intX = IntX(3);
	BOOST_CHECK(intX - 5 == -2);
}

BOOST_AUTO_TEST_CASE(SubIntXInt)
{
	IntX intX = IntX(3);
	BOOST_CHECK((IntX)5 - intX == 2);
}

BOOST_AUTO_TEST_CASE(Sub0IntX)
{
	IntX int1 = IntX(3);
	BOOST_CHECK(int1 - 0 == 3);
	BOOST_CHECK((IntX)0 - int1 == -3);
	BOOST_CHECK(int1 - IntX() == 3);
	BOOST_CHECK(IntX() - int1 == -3);
}

BOOST_AUTO_TEST_CASE(Sub0IntXNeg)
{
	IntX int1 = IntX(-3);
	BOOST_CHECK(int1 - 0 == -3);
	BOOST_CHECK((IntX)0 - int1 == 3);
	BOOST_CHECK(int1 - IntX() == -3);
	BOOST_CHECK(IntX() - int1 == 3);
}

BOOST_AUTO_TEST_CASE(Sub2BigIntX)
{
	IntX int1 = IntX(vector<UInt32>({ 1, 2, 3 }), false);
	IntX int2 = IntX(vector<UInt32>({ 3, 4, 5 }), false);
	IntX int3 = IntX(vector<UInt32>({ 2, 2, 2 }), true);
	BOOST_CHECK(int1 - int2 == int3);
}

BOOST_AUTO_TEST_CASE(Sub2BigIntXC)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value - 1 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ 0, 0, 1 }), false);
	BOOST_CHECK(int1 == int3 - int2);
}

BOOST_AUTO_TEST_CASE(Sub2BigIntXC2)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value - 1, Constants::MaxUInt32Value - 1 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value }), false);
	BOOST_CHECK(int1 == int3 - int2);
}

BOOST_AUTO_TEST_CASE(Sub2BigIntXC3)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ 0, 1, 1 }), false);
	BOOST_CHECK(int1 == int3 - int2);
}

BOOST_AUTO_TEST_CASE(Sub2BigIntXC4)
{
	IntX int1 = IntX(vector<UInt32>({ Constants::MaxUInt32Value, Constants::MaxUInt32Value, 1, 1 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 1 }), false);
	IntX int3 = IntX(vector<UInt32>({ 0, 1, 2, 1 }), false);
	BOOST_CHECK(int1 == int3 - int2);
}

BOOST_AUTO_TEST_CASE(SubAdd)
{
	IntX int1 = IntX(2);
	IntX int2 = IntX(-3);
	BOOST_CHECK(int1 - int2 == 5);
}

BOOST_AUTO_TEST_SUITE_END()

