#define BOOST_TEST_MODULE ConstructorTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ConstructorTest)

BOOST_AUTO_TEST_CASE(DefaultCtor)
{
	IntX();
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(IntCtor)
{
	IntX(7);
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(UIntCtor)
{
	IntX((UInt32)Constants::MaxUInt32Value);
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(IntArrayCtor)
{
	IntX(vector<UInt32>({ 1, 2, 3 }), true);
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

