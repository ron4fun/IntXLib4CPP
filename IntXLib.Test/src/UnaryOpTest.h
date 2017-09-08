#define BOOST_TEST_MODULE UnaryOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(UnaryOpTest)

BOOST_AUTO_TEST_CASE(Plus)
{
	IntX intX = 77;
	BOOST_CHECK(intX == +intX);
}

BOOST_AUTO_TEST_CASE(Minus)
{
	IntX intX = 77;
	BOOST_CHECK(-intX == -77);
}

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX intX = 0;
	BOOST_CHECK(intX == +intX);
	BOOST_CHECK(intX == -intX);
}

BOOST_AUTO_TEST_CASE(PlusPlus)
{
	IntX intX = 77;
	BOOST_CHECK(intX++ == 77);
	BOOST_CHECK(++intX == 79);
}

BOOST_AUTO_TEST_CASE(MinusMinus)
{
	IntX intX = 77;
	BOOST_CHECK(intX-- == 77);
	BOOST_CHECK(--intX == 75);
}

BOOST_AUTO_TEST_SUITE_END()
