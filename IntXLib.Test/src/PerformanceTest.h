#define BOOST_TEST_MODULE PerformanceTest

#include "../IntX.h"
#include <vector>
#include <Windows.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(PerformanceTest)

BOOST_AUTO_TEST_CASE(Multiply128BitNumbers)
{
	IntX int1 = IntX(vector<UInt32>({ 47668, 58687, 223234234, 42424242 }), false);
	IntX int2 = IntX(vector<UInt32>({ 5674356, 34656476, 45667, 678645646 }), false);

	double startwatch = GetTickCount();

	for (register UInt32 i = 0; i < 100000; ++i)
	{
		IntX::Multiply(int1, int2, MultiplyMode::mmClassic);
	} // end for

	double endwatch = GetTickCount();

	//return endwatch - startwatch;
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

