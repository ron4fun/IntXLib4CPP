#define BOOST_TEST_MODULE ImplicitConvertOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ImplicitConvertOpTest)

BOOST_AUTO_TEST_CASE(ConvertAllExceptLong)
{
	IntX int1 = int();
	BOOST_CHECK(int1 == 0);
	int1 = UInt32();
	BOOST_CHECK(int1 == 0);
	int1 = unsigned char();
	BOOST_CHECK(int1 == 0);
	int1 = char();
	BOOST_CHECK(int1 == 0);
	int1 = short();
	BOOST_CHECK(int1 == 0);
	int1 = unsigned short();
	BOOST_CHECK(int1 == 0);
}

BOOST_AUTO_TEST_CASE(ConvertLong)
{
	IntX int1 = long();
	BOOST_CHECK(int1 == 0);
	int1 = UInt64();
	BOOST_CHECK(int1 == 0);
	int1 = -123123123123;
	BOOST_CHECK(int1 == -123123123123);
}

BOOST_AUTO_TEST_SUITE_END()

