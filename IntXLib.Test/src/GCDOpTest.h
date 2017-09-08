#define BOOST_TEST_MODULE GCDOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(GCDOpTest)

BOOST_AUTO_TEST_CASE(GCDIntXBothPositive)
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

BOOST_AUTO_TEST_CASE(GCDIntXBothNegative)
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

BOOST_AUTO_TEST_CASE(GCDIntXBothSigns)
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

