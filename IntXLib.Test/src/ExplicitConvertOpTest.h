#define BOOST_TEST_MODULE ExplicitConvertOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ExplicitConvertOpTest)

BOOST_AUTO_TEST_CASE(ConvertToInt)
{
	int n = 1234567890;
	IntX intX = n;
	BOOST_CHECK(n == (int)intX);

	n = -n;
	intX = n;
	BOOST_CHECK(n == (int)intX);

	n = 0;
	intX = n;
	BOOST_CHECK(n == (int)intX);

	n = 1234567890;
	UInt32 un = (UInt32)n;
	intX = IntX(vector<UInt32>({ un, un, un }), false);

	BOOST_CHECK(n == (int)intX);

	intX = IntX(vector<UInt32>({ un, un, un }), true);
	BOOST_CHECK(-n == (int)intX);
}

BOOST_AUTO_TEST_CASE(ConvertToUInt)
{
	UInt32 n = 1234567890;
	IntX intX = n;
	BOOST_CHECK(n == (UInt32)intX);

	n = 0;
	intX = n;
	BOOST_CHECK(n == (UInt32)intX);

	n = 1234567890;
	intX = IntX(vector<UInt32>({ n, n, n }), false);
	BOOST_CHECK(n == (UInt32)intX);
}

BOOST_AUTO_TEST_CASE(ConvertToInt64)
{
	long long n = 1234567890123456789;
	IntX intX = n;
	BOOST_CHECK(n == (long long)intX);

	n = -n;
	intX = n;
	BOOST_CHECK(n == (long long)intX);

	n = 0;
	intX = n;
	BOOST_CHECK(n == (long long)intX);

	UInt32 un = 1234567890;
	n = (long long)(un | (UInt64)un << 32);
	intX = IntX(vector<UInt32>({ un, un, un, un, un }), false);
	BOOST_CHECK(n == (long long)intX);

	intX = IntX(vector<UInt32>({ un, un, un, un, un }), true);
	BOOST_CHECK(-n == (long long)intX);

	int ni = 1234567890;
	n = ni;
	intX = ni;
	BOOST_CHECK(n == (long long)intX);
}

BOOST_AUTO_TEST_CASE(ConvertToUInt64)
{
	UInt64 n = 1234567890123456789;
	IntX intX = n;
	BOOST_CHECK(n == (UInt64)intX);

	n = 0;
	intX = n;
	BOOST_CHECK(n == (UInt64)intX);

	UInt32 un = 1234567890;
	n = un | (UInt64)un << 32;
	intX = IntX(vector<UInt32>({ un, un, un, un, un }), false);
	BOOST_CHECK(n == (UInt64)intX);

	n = un;
	intX = un;
	BOOST_CHECK(n == (UInt64)intX);
}

BOOST_AUTO_TEST_CASE(ConvertToUshort)
{
	unsigned short n = 12345;
	IntX intX = n;
	BOOST_CHECK(n == (unsigned short)intX);

	n = 0;
	intX = n;
	BOOST_CHECK(n == (unsigned short)intX);
}

BOOST_AUTO_TEST_SUITE_END()