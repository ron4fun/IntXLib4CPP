#define BOOST_TEST_MODULE IntegerLogNTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(IntegerLogNTest)

BOOST_AUTO_TEST_CASE(LogNBase10)
{
	IntX base, number;

	base = 10;
	number = 100;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 2);

	number = 10;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 1);

	number = 500;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 2);

	number = 1000;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 3);
}

BOOST_AUTO_TEST_CASE(LogNBase2)
{
	IntX base, number;

	base = 2;
	number = 100;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 6);

	number = 10;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 3);

	number = 500;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 8);

	number = 1000;
	BOOST_CHECK(IntX::IntegerLogN(base, number) == 9);
}

BOOST_AUTO_TEST_SUITE_END()

