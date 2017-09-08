#define BOOST_TEST_MODULE OpEqTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(OpEqTest)

BOOST_AUTO_TEST_CASE(Plus)
{
	IntX intX = 77;
	IntX  ans = intX + intX;

	intX += 77;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(Minus)
{
	IntX intX = 77;
	IntX  ans = intX - 7;

	intX -= 7;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(Mul)
{
	IntX intX = 45;
	IntX  ans = intX * 7;

	intX *= 7;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(Div)
{
	IntX intX = 27;
	IntX  ans = intX / 7;

	intX /= 7;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(Rem)
{
	IntX intX = 77;
	IntX  ans = intX % 5;

	intX %= 5;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(Or)
{
	IntX intX = 45;
	IntX  ans = intX | 23;

	intX |= 23;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(And)
{
	IntX intX = 45;
	IntX  ans = intX & 53;

	intX &= 53;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_CASE(Xor)
{
	IntX intX = 27;
	IntX  ans = intX ^ 7;

	intX ^= 7;
	BOOST_CHECK(intX == ans);
}

BOOST_AUTO_TEST_SUITE_END()
