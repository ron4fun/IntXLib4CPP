#define BOOST_TEST_MODULE IsOddTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(IsOddTest)

BOOST_AUTO_TEST_CASE(ShouldBeFalseForZero)
{
	IntX value = IntX();

	bool result = value.IsOdd();

	BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(ShouldBeFalseForEvenNumber)
{
	IntX value = IntX(42);

	bool result = value.IsOdd();

	BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(ShouldBeTrueForOddNumber)
{
	IntX value = IntX(57);

	bool result = value.IsOdd();

	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()

