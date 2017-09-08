#define BOOST_TEST_MODULE BezoutsIdentityOpTest

#include "../IntX.h"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BezoutsIdentityOpTest)

BOOST_AUTO_TEST_CASE(BezIntX)
{
	IntX gcd, bez1, bez2;

	gcd = IntX::Bezoutsidentity(421, 111, bez1, bez2);
	BOOST_CHECK(bez1 == -29);
	BOOST_CHECK(bez2 == 110);
	BOOST_CHECK(gcd == 1);

	gcd = IntX::Bezoutsidentity(93, 219, bez1, bez2);
	BOOST_CHECK(bez1 == 33);
	BOOST_CHECK(bez2 == -14);
	BOOST_CHECK(gcd == 3);
}

BOOST_AUTO_TEST_SUITE_END()

