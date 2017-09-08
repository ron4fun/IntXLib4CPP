#define BOOST_TEST_MODULE InvModOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(InvModOpTest)

BOOST_AUTO_TEST_CASE(IntXBothPositive)
{
	IntX res;

	res = IntX::InvMod(123, 4567);
	BOOST_CHECK(res == 854);

	res = IntX::InvMod(9876, 2457);
	BOOST_CHECK(res == 0);
}

BOOST_AUTO_TEST_SUITE_END()

