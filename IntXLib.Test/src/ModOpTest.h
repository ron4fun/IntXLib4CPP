#define BOOST_TEST_MODULE ModOpTest

#include "../IntX.h"
#include "../Utils/Utils.h"
#include <stdexcept>
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ModOpTest)

BOOST_AUTO_TEST_CASE(Simple)
{
	IntX int1 = 16;
	IntX int2 = 5;
	BOOST_CHECK(int1 % int2 == 1);
}

BOOST_AUTO_TEST_CASE(Neg)
{
	IntX int1 = -16;
	IntX int2 = 5;
	BOOST_CHECK(int1 % int2 == -1);

	int1 = 16;
	int2 = -5;
	BOOST_CHECK(int1 % int2 == 1);

	int1 = -16;
	int2 = -5;
	BOOST_CHECK(int1 % int2 == -1);
}

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX int1 = 0;
	IntX int2 = 25;
	BOOST_CHECK(int1 % int2 == 0);

	int1 = 0;
	int2 = -25;
	BOOST_CHECK(int1 % int2 == 0);

	int1 = 16;
	int2 = 25;
	BOOST_CHECK(int1 % int2 == 16);

	int1 = -16;
	int2 = 25;
	BOOST_CHECK(int1 % int2 == -16);

	int1 = 16;
	int2 = -25;
	BOOST_CHECK(int1 % int2 == 16);

	int1 = -16;
	int2 = -25;
	BOOST_CHECK(int1 % int2 == -16);

	int1 = 50;
	int2 = 25;
	BOOST_CHECK(int1 % int2 == 0);

	int1 = -50;
	int2 = -25;
	BOOST_CHECK(int1 % int2 == 0);
}

BOOST_AUTO_TEST_CASE(ZeroException)
{
	try
	{
		IntX int1 = 0;
		IntX int2 = 0;
		int1 = int1 % int2;
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
	IntX intM = IntX(vector<UInt32>({ 2, 0xffffffffU, 0x7fffffffU }), false);

	BOOST_CHECK(int1 % int2 == intM);
}

BOOST_AUTO_TEST_CASE(BigDec)
{
	IntX int1 = IntX("100000000000000000000000000000000000000000000");
	IntX int2 = IntX("100000000000000000000000000000000000000000");
	BOOST_CHECK(int1 % int2 == 0);
}

BOOST_AUTO_TEST_CASE(BigDecNeg)
{
	IntX int1 = IntX("-100000000000000000000000000000000000000000001");
	IntX int2 = IntX("100000000000000000000000000000000000000000");
	BOOST_CHECK(int1 % int2 == -1);
}

BOOST_AUTO_TEST_SUITE_END()

