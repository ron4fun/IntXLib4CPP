#define BOOST_TEST_MODULE ShiftOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ShiftOpTest)

BOOST_AUTO_TEST_CASE(Zero)
{
	IntX int1 = IntX();
	BOOST_CHECK(int1 << 100 == 0);
	BOOST_CHECK(int1 >> 100 == 0);
}

BOOST_AUTO_TEST_CASE(SimpleAndNeg)
{
	IntX int1 = IntX(8);
	BOOST_CHECK(int1 << 0 == int1 >> 0 && int1 << 0 == 8);
	BOOST_CHECK(int1 << 32 == int1 >> -32 && int1 << 32 == IntX(vector<UInt32>({ 0, 8 }), false));
}

BOOST_AUTO_TEST_CASE(Complex)
{
	IntX int1 = IntX("0x0080808080808080");
	BOOST_CHECK((int1 << 4).ToString(16) == "808080808080800");
	BOOST_CHECK(int1 >> 36 == 0x80808);
}

BOOST_AUTO_TEST_CASE(BigShift)
{
	IntX int1 = 8;
	BOOST_CHECK(int1 >> 777 == 0);
}

BOOST_AUTO_TEST_CASE(MassiveShift)
{
	for (int i = 1; i < 2000; i++)
	{
		IntX n = i;
		n = n << i;
		n = n >> i;
		BOOST_CHECK(IntX(i) == n);
	} // end for
}

BOOST_AUTO_TEST_SUITE_END()
