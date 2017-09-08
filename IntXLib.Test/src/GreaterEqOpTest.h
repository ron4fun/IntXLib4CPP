#define BOOST_TEST_MODULE GreaterEqOpTest

#include "../IntX.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(GreaterEqOpTest)

BOOST_AUTO_TEST_CASE(Simple)
{
	IntX int1 = IntX(7);
	IntX int2 = IntX(8);
	BOOST_CHECK(int2 >= int1);
	BOOST_CHECK(8 >= int1);
	BOOST_CHECK(int2 >= 7);
}

BOOST_AUTO_TEST_CASE(SimpleFail)
{
	IntX int1 = IntX(8);
	BOOST_CHECK(!(7 >= int1));
}

BOOST_AUTO_TEST_CASE(Big)
{
	IntX int1 = IntX(vector<UInt32>({ 1, 2 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 2, 3 }), true);

	BOOST_CHECK(int1 >= int2);
}

BOOST_AUTO_TEST_CASE(BigFail)
{
	IntX int1 = IntX(vector<UInt32>({ 1, 2 }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 2, 3 }), true);

	BOOST_CHECK(!(int2 >= int1));
}

BOOST_AUTO_TEST_CASE(EqualValues)
{
	IntX int1 = IntX(vector<UInt32>({ 1, 2, 3 }), true);
	IntX int2 = IntX(vector<UInt32>({ 1, 2, 3 }), true);
	BOOST_CHECK(int2 >= int1);
}

BOOST_AUTO_TEST_SUITE_END()
