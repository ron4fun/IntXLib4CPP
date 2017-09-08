#define BOOST_TEST_MODULE IntegerSquareRootTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(IntegerSquareRootTest)

BOOST_AUTO_TEST_CASE(SquareRootOfZero)
{
	IntX int1 = 0;
	BOOST_CHECK(IntX::IntegerSquareRoot(int1) == 0);
}

BOOST_AUTO_TEST_CASE(SquareRootOfOne)
{
	IntX int1 = 1;
	BOOST_CHECK(IntX::IntegerSquareRoot(int1) == 1);
}

BOOST_AUTO_TEST_CASE(SquareRootof4)
{
	IntX int1 = 4;
	BOOST_CHECK(IntX::IntegerSquareRoot(int1) == 2);
}

BOOST_AUTO_TEST_CASE(SquareRootof25)
{
	IntX int1 = 25;
	IntX res = IntX::IntegerSquareRoot(int1);
	BOOST_CHECK(res == 5);
}

BOOST_AUTO_TEST_CASE(SquareRootof27)
{
	IntX int1 = 27;
	IntX res = IntX::IntegerSquareRoot(int1);
	BOOST_CHECK(res == 5);
}

BOOST_AUTO_TEST_CASE(SquareRootofVeryBigValue)
{
	IntX int1 = "783648276815623658365871365876257862874628734627835648726";
	BOOST_CHECK(IntX::IntegerSquareRoot(int1).ToString() == "27993718524262253829858552106");
}

BOOST_AUTO_TEST_CASE(SquareRootofNegativeNumber)
{
	try
	{
		IntX int1 = -25;
		IntX::IntegerSquareRoot(int1);
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(ArgumentException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END()
