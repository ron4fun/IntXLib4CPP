#define BOOST_TEST_MODULE MulOpTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <string>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MulOpTest)

BOOST_AUTO_TEST_CASE(PureIntX)
{
	BOOST_CHECK(IntX(3) * IntX(5) == IntX(15));
}

BOOST_AUTO_TEST_CASE(PureIntXSign)
{
	BOOST_CHECK(IntX(-3) * IntX(5) == IntX(-15));
}

BOOST_AUTO_TEST_CASE(IntAndIntX)
{
	BOOST_CHECK(IntX(3) * 5 == 15);
}

BOOST_AUTO_TEST_CASE(Zero)
{
	BOOST_CHECK((IntX)0 * IntX(3) == 0);
}

BOOST_AUTO_TEST_CASE(Big)
{
	vector<UInt32> temp1, temp2, tempRes;
	IntX int1, int2, intRes;

	temp1.resize(2);
	temp1[0] = 1;
	temp1[1] = 1;
	temp2.resize(2);
	temp2[0] = 1;
	temp2[1] = 1;
	tempRes.resize(3);
	tempRes[0] = 1;
	tempRes[1] = 2;
	tempRes[2] = 1;

	int1 = IntX(temp1, false);
	int2 = IntX(temp2, false);
	intRes = IntX(tempRes, false);

	BOOST_CHECK(int1 * int2 == intRes);
}

BOOST_AUTO_TEST_CASE(Big2)
{
	vector<UInt32> temp1, temp2, tempRes;
	IntX int1, int2, intRes;

	temp1.resize(2);
	temp1[0] = 1;
	temp1[1] = 1;
	temp2.resize(1);
	temp2[0] = 2;
	tempRes.resize(2);
	tempRes[0] = 2;
	tempRes[1] = 2;

	int1 = IntX(temp1, false);
	int2 = IntX(temp2, false);
	intRes = IntX(tempRes, false);

	BOOST_CHECK(intRes == int1 * int2);
	BOOST_CHECK(intRes == int2 * int1);
}

BOOST_AUTO_TEST_CASE(Big3)
{
	vector<UInt32> temp1, temp2, tempRes;
	IntX int1, int2, intRes;

	temp1.resize(2);
	temp1[0] = Constants::MaxUInt32Value;
	temp1[1] = Constants::MaxUInt32Value;
	temp2.resize(2);
	temp2[0] = Constants::MaxUInt32Value;
	temp2[1] = Constants::MaxUInt32Value;
	tempRes.resize(4);
	tempRes[0] = 1;
	tempRes[1] = 0;
	tempRes[2] = Constants::MaxUInt32Value - 1;
	tempRes[3] = Constants::MaxUInt32Value;

	int1 = IntX(temp1, false);
	int2 = IntX(temp2, false);
	intRes = IntX(tempRes, false);

	BOOST_CHECK(int1 * int2 == intRes);
}

BOOST_AUTO_TEST_CASE(Performance)
{
	int i;
	vector<UInt32> temp1;
	IntX intX1, intX2;

	temp1.resize(2);
	temp1[0] = 0;
	temp1[1] = 1;

	intX1 = IntX(temp1, false);
	intX2 = intX1;

	i = 0;
	while (i < 1000)
	{
		intX2 = intX2 * intX1;
		++i;
	} // end while

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
	