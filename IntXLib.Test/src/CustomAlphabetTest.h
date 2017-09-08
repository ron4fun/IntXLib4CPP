#define BOOST_TEST_MODULE ConstructorTest

#include "../IntX.h"
#include "../Utils/Constants.h"
#include <vector>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CustomAlphabetTest)

BOOST_AUTO_TEST_CASE(AlphabetNull)
{
	try
	{
		IntX::Parse("", 20, "");
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(ArgumentNullException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(AlphabetShort)
{
	try
	{
		IntX::Parse("", 20, "1234");
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(ArgumentException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(AlphabetRepeatingChars)
{
	try
	{
		IntX::Parse("", 20, "0123456789ABCDEFGHIJ0");
	} // end try
	catch (const exception&e)
	{
		BOOST_CHECK(typeid(e) == typeid(ArgumentException));
		return;
	} // end catch

	BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(Parse)
{
	BOOST_CHECK(19 * 20 + 18 == (int)IntX::Parse("JI", 20, "0123456789ABCDEFGHIJ"));
}

BOOST_AUTO_TEST_CASE(ToStringTest)
{
	BOOST_CHECK("JI" == IntX(19 * 20 + 18).ToString(20, string("0123456789ABCDEFGHIJ")));
}

BOOST_AUTO_TEST_SUITE_END()
