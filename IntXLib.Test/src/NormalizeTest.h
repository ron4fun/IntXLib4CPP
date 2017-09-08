#define BOOST_TEST_MODULE NormalizeTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(NormalizeTest)

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX int1 = IntX(7) - 7;
	int1.Normalize();
	BOOST_CHECK(int1 == 0);
}

BOOST_AUTO_TEST_CASE(Simple)
{
	IntX int1 = IntX(8);
	int1 = int1 * int1;
	int1.Normalize();
	BOOST_CHECK(int1 == 64);
}

BOOST_AUTO_TEST_SUITE_END()
