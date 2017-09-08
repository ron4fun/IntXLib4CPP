#define BOOST_TEST_MODULE DivOpTest

#include "../IntX.h"
#include "../Utils/Utils.h"
#include <vector>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(DivOpTest)

BOOST_AUTO_TEST_CASE(Simple)
{
	IntX int1 = 16;
	IntX int2 = 5;
	BOOST_CHECK(int1 / int2 == 3);
}

BOOST_AUTO_TEST_CASE(Neg)
{
	IntX int1 = -16;
	IntX int2 = 5;
	BOOST_CHECK(int1 / int2 == -3);

	int1 = 16;
	int2 = -5;
	BOOST_CHECK(int1 / int2 == -3);

	int1 = -16;
	int2 = -5;
	BOOST_CHECK(int1 / int2 == 3);
}

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX int1 = 0;
	IntX int2 = 25;
	BOOST_CHECK(int1 / int2 == 0);

	int1 = 0;
	int2 = -25;
	BOOST_CHECK(int1 / int2 == 0);

	int1 = 16;
	int2 = 25;
	BOOST_CHECK(int1 / int2 == 0);

	int1 = -16;
	int2 = 25;
	BOOST_CHECK(int1 / int2 == 0);

	int1 = 16;
	int2 = -25;
	BOOST_CHECK(int1 / int2 == 0);

	int1 = -16;
	int2 = -25;
	BOOST_CHECK(int1 / int2 == 0);
}

BOOST_AUTO_TEST_CASE(ZeroException)
{
	try
	{
		IntX int1 = 0;
		IntX int2 = 0;
		int1 = int1 / int2;
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(DivideByZeroException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(Big)
{
	IntX int1 = IntX(vector<UInt32>({ 0, 0, 0x80000000U, 0x7fffffffU }), false);
	IntX int2 = IntX(vector<UInt32>({ 1, 0, 0x80000000U }), false);
	BOOST_CHECK(int1 / int2 == 0xfffffffeU);
}

BOOST_AUTO_TEST_CASE(Big2)
{
	IntX int1 = IntX("4574586690780877990306040650779005020012387464357");
	IntX int2 = IntX("856778798907978995905496597069809708960893");
	IntX int3 = IntX("8567787989079799590496597069809708960893");
	IntX int4 = int1 * int2 + int3;

	BOOST_CHECK(int4 / int2 == int1);
	BOOST_CHECK(int4 % int2 == int3);
}

BOOST_AUTO_TEST_CASE(BigDec)
{
	IntX int1 = IntX("100000000000000000000000000000000000000000000");
	IntX int2 = IntX("100000000000000000000000000000000000000000");

	BOOST_CHECK(int1 / int2 == 1000);
}

BOOST_AUTO_TEST_CASE(BigDecNeg)
{
	IntX int1 = IntX("-100000000000000000000000000000000000000000000");
	IntX int2 = IntX("100000000000000000000000000000000000000000");

	BOOST_CHECK(int1 / int2 == -1000);
}

BOOST_AUTO_TEST_SUITE_END()
