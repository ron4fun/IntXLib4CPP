#define BOOST_TEST_MODULE IsProbablyPrimeOpTest

#include "../IntX.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(IsProbablyPrimeOpTest)

BOOST_AUTO_TEST_CASE(IsPrimeIntX)
{
	BOOST_CHECK(IntX::IsProbablyPrime(2));
	BOOST_CHECK(IntX::IsProbablyPrime(373));
	BOOST_CHECK(IntX::IsProbablyPrime(743));
	BOOST_CHECK(IntX::IsProbablyPrime(991));
}

BOOST_AUTO_TEST_CASE(IsNotPrimeIntX)
{
	BOOST_CHECK(!IntX::IsProbablyPrime(0));
	BOOST_CHECK(!IntX::IsProbablyPrime(1));
	BOOST_CHECK(!IntX::IsProbablyPrime(908));
	BOOST_CHECK(!IntX::IsProbablyPrime(992));
}

BOOST_AUTO_TEST_CASE(IsPrimeBigIntX)
{
	IntX Prime;

	Prime = "40378229068348060265902071710277325464903647442059563624162746921011818446300065249638243017389009856122930741656904767";
	BOOST_CHECK(IntX::IsProbablyPrime(Prime));
}

BOOST_AUTO_TEST_SUITE_END()
