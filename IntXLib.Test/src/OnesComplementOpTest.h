#define BOOST_TEST_MODULE OnesComplementOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(OnesComplementOpTest)

BOOST_AUTO_TEST_CASE(ShouldOnesComplementIntX)
{
	IntX value = IntX(11);

	IntX result = ~value;

	long long temp = -(long long)(~(UInt32)11);
	BOOST_CHECK(result == temp);
}

BOOST_AUTO_TEST_CASE(ShouldOnesComplementNegativeIntX)
{
	IntX value = IntX(-11);

	IntX result = ~value;

	BOOST_CHECK(result == ~(UInt32)11);
}

BOOST_AUTO_TEST_CASE(ShouldOnesComplementZero)
{
	IntX value = IntX();

	IntX result = ~value;

	BOOST_CHECK(result == 0);
}

BOOST_AUTO_TEST_CASE(ShouldOnesComplementBigIntX)
{
	IntX value = IntX(vector<UInt32>({ 3, 5, Constants::MaxUInt32Value }), false);

	IntX result = ~value;

	BOOST_CHECK(result == IntX(vector<UInt32>({ ~(UInt32)3, ~(UInt32)5 }), true));
}

BOOST_AUTO_TEST_SUITE_END()
