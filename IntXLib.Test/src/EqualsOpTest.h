#define BOOST_TEST_MODULE EqualsOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(EqualsOpTest)

BOOST_AUTO_TEST_CASE(Equals2IntX)
{
	IntX int1 = IntX(8);
	IntX int2 = IntX(8);
	BOOST_CHECK(int1.Equals(int2));
}

BOOST_AUTO_TEST_CASE(EqualsZeroIntX)
{
	BOOST_CHECK(IntX(0) != 1);
}

BOOST_AUTO_TEST_CASE(EqualsIntIntX)
{
	IntX int1 = IntX(8);
	BOOST_CHECK(8 == int1);
}

BOOST_AUTO_TEST_CASE(EqualsIntXInt)
{
	IntX int1 = IntX(8);
	BOOST_CHECK(int1 == 8);
}

BOOST_AUTO_TEST_CASE(EqualsNullIntX)
{
	IntX int1 = IntX(8);
	BOOST_CHECK(int1 != IntX());
}

BOOST_AUTO_TEST_CASE(Equals2IntXOp)
{
	IntX int1 = IntX(8);
	IntX int2 = IntX(8);
	BOOST_CHECK(int1 == int2);
}

BOOST_AUTO_TEST_SUITE_END()
