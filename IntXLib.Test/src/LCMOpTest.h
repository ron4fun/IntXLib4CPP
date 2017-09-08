#define BOOST_TEST_MODULE LCMOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(LCMOpTest)

BOOST_AUTO_TEST_CASE(LCMIntXBothPositive)
{
	IntX res;

	res = IntX::GCD(4, 6);
	BOOST_CHECK(res == 2);
	res = IntX::GCD(24, 18);
	BOOST_CHECK(res == 6);
	res = IntX::GCD(234, 100);
	BOOST_CHECK(res == 2);
	res = IntX::GCD(235, 100);
	BOOST_CHECK(res == 5);
}

BOOST_AUTO_TEST_CASE(LCMIntXBothNegative)
{
	IntX res;

	res = IntX::GCD(-4, -6);
	BOOST_CHECK(res == 2);
	res = IntX::GCD(-24, -18);
	BOOST_CHECK(res == 6);
	res = IntX::GCD(-234, -100);
	BOOST_CHECK(res == 2);
	res = IntX::GCD(-235, -100);
	BOOST_CHECK(res == 5);
}

BOOST_AUTO_TEST_CASE(LCMIntXBothSigns)
{
	IntX res;

	res = IntX::GCD(-4, +6);
	BOOST_CHECK(res == 2);
	res = IntX::GCD(+24, -18);
	BOOST_CHECK(res == 6);
	res = IntX::GCD(-234, +100);
	BOOST_CHECK(res == 2);
	res = IntX::GCD(+235, -100);
	BOOST_CHECK(res == 5);
}

BOOST_AUTO_TEST_SUITE_END()
