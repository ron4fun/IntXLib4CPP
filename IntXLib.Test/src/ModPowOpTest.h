#define BOOST_TEST_MODULE ModPowOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ModPowOpTest)

BOOST_AUTO_TEST_CASE(ModPowIntX)
{
	IntX res;

	res = IntX::ModPow(11, 13, 19);
	BOOST_CHECK(res == 11);

	res = IntX::ModPow(123, 4567, 789);
	BOOST_CHECK(res == 687);

	res = IntX::ModPow(9876, 2457, 6457);
	BOOST_CHECK(res == 3238);
}

BOOST_AUTO_TEST_SUITE_END()
