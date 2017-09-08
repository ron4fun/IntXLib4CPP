#define BOOST_TEST_MODULE AbsoluteValueTest

#include "../IntX.h"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(AbsoluteValueTest)

BOOST_AUTO_TEST_CASE(AbsoluteTest)
{
	IntX int1, res;

	int1 = -5;
	res = IntX::AbsoluteValue(int1);
	BOOST_CHECK(res == 5);

	res = IntX::AbsoluteValue(-25);
	BOOST_CHECK(res == 25);

	res = IntX::AbsoluteValue(IntX::Parse("-500"));
	BOOST_CHECK(res == 500);

	int1 = 10;
	res = IntX::AbsoluteValue(int1);
	BOOST_CHECK(res == 10);

	res = IntX::AbsoluteValue(80);
	BOOST_CHECK(res == 80);

	res = IntX::AbsoluteValue(IntX::Parse("900"));
	BOOST_CHECK(res == 900);
}

BOOST_AUTO_TEST_CASE(AbsoluteTestZero)
{
	IntX int1, res;

	int1 = IntX(-0);
	res = IntX::AbsoluteValue(int1);
	BOOST_CHECK(res == 0);

	res = IntX::AbsoluteValue(IntX::Parse("-0"));
	BOOST_CHECK(res == 0);

	res = IntX::AbsoluteValue(0);
	BOOST_CHECK(res == 0);

	res = IntX::AbsoluteValue(IntX::Parse("0"));
	BOOST_CHECK(res == 0);
}

BOOST_AUTO_TEST_SUITE_END()

