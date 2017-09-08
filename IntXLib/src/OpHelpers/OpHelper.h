#pragma once

#ifndef OPHELPER_H
#define OPHELPER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "../MillerRabin/MillerRabin.h"
#include "../PcgRandom/PcgRandomMinimal.h"
#include "../Multipliers/IMultiplier.h"
#include "../Multipliers/MultiplyManager.h"
#include "DigitOpHelper.h"
#include "DigitHelper.h"
#include "../Utils/Constants.h"
#include "../Utils/Utils.h"
#include "../Utils/Strings.h"
#include "Bits.h"
#include "IntX.h"

#include <cmath>

using namespace std;

class OpHelper
{
public:

	/// <summary>
	/// Determines <see cref="IntX" /> object with lower length.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="smallerInt">Resulting smaller big integer (by length only).</param>
	/// <param name="biggerInt">Resulting bigger big integer (by length only).</param>
	static void GetMinMaxLengthObjects(const IntX &int1, const IntX &int2, IntX &smallerInt, IntX &biggerInt)
	{
		if (int1.length < int2.length)
		{
			smallerInt = IntX(int1);
			biggerInt = IntX(int2);
		} // end if
		else
		{
			smallerInt = IntX(int2);
			biggerInt = IntX(int1);
		} // end else
	} // end function GetMinMaxLengthObjects

	/// <summary>
	/// Adds two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	/// <exception cref="ArgumentException"><paramref name="int1" /> or <paramref name="int2" /> is too big for add operation.</exception>
	static IntX Add(const IntX &int1, const IntX &int2)
	{
		// Process zero values in special way
		if (int2.length == 0) return int1;
		if (int1.length == 0)
		{
			IntX x = int2;
			x.negative = int1.negative; // always get sign of the first big integer
			return x;
		} // end if

		  // Determine big int with lower length
		IntX smallerInt;
		IntX biggerInt;
		GetMinMaxLengthObjects(int1, int2, smallerInt, biggerInt);

		// Check for add operation possibility
		if (biggerInt.length == Constants::MaxIntValue)
		{
			throw ArgumentException(Strings::IntegerTooBig);
		} // end if

		  // Create new big int object of needed length
		IntX newInt = IntX(biggerInt.length + 1, int1.negative);

		// Do actual addition
		newInt.length = DigitOpHelper::Add(
			&biggerInt.digits[0],
			biggerInt.length,
			&smallerInt.digits[0],
			smallerInt.length,
			&newInt.digits[0]);

		// Normalization may be needed
		newInt.TryNormalize();

		return newInt;
	} // end function Add

	/// <summary>
	/// Subtracts two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	static IntX Sub(const IntX &int1, const IntX &int2)
	{
		// Process zero values in special way
		if (int1.length == 0) return IntX(int2.digits, true, int2.length);
		if (int2.length == 0) return IntX(int1);

		// Determine lower big int (without sign)
		IntX smallerInt;
		IntX biggerInt;
		int compareResult = DigitOpHelper::Cmp(&int1.digits[0], int1.length, &int2.digits[0], int2.length);
		if (compareResult == 0) return IntX(); // integers are equal
		if (compareResult < 0)
		{
			smallerInt = IntX(int1);
			biggerInt = IntX(int2);
		} // end if
		else
		{
			smallerInt = IntX(int2);
			biggerInt = IntX(int1);
		} // end else

		  // Create new big int object
		IntX newInt = IntX(biggerInt.length, IntX::ReferenceEquals(int1, smallerInt) ^ int1.negative);

		// Do actual subtraction
		newInt.length = DigitOpHelper::Sub(
			&biggerInt.digits[0],
			biggerInt.length,
			&smallerInt.digits[0],
			smallerInt.length,
			&newInt.digits[0]);

		// Normalization may be needed
		newInt.TryNormalize();

		return newInt;
	} // end function Sub

	/// <summary>
	/// Adds/subtracts one <see cref="IntX" /> to/from another.
	/// Determines which operation to use basing on operands signs.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="subtract">Was subtraction initially.</param>
	/// <returns>Add/subtract operation result.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="int1" /> or <paramref name="int2" /> is a null reference.</exception>
	static IntX AddSub(const IntX &int1, const IntX &int2, const bool subtract)
	{
		// Determine real operation type and result sign
		return ((subtract ^ int1.negative) == int2.negative) ? Add(int1, int2) : Sub(int1, int2);
	} // end function AddSub

	/// <summary>
	/// Returns a specified big integer raised to the specified power.
	/// </summary>
	/// <param name="value">Number to raise.</param>
	/// <param name="power">Power.</param>
	/// <param name="multiplyMode">Multiply mode set explicitly.</param>
	/// <returns>Number in given power.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="value" /> is a null reference.</exception>
	static IntX Pow(const IntX &value, const UInt32 power, MultiplyMode multiplyMode)
	{
		// Return one for zero pow
		if (power == 0) return IntX(1);

		// Return the number itself from a power of one
		if (power == 1) return IntX(value);

		// Return zero for a zero
		if (value.length == 0) return IntX();

		// optimization if value (base) is 2.
		if (value == 2)
		{
			return IntX(1) << power;
		} // end if

		// Get first one bit
		int msb = Bits::Msb(power);

		// Get multiplier
		IMultiplier *multiplier = MultiplyManager::GetMultiplier(multiplyMode);

		// Do actual raising
		IntX res = value;
		for (UInt32 powerMask = 1U << (msb - 1); powerMask != 0; powerMask >>= 1)
		{
			// Always square
			res = multiplier->Multiply(res, res);

			// Maybe mul
			if ((power & powerMask) != 0)
			{
				res = multiplier->Multiply(res, value);
			} // end if
		} // end for

		return res;
	} // end function Pow

	//==================================================================
	//  Thanks to Xor-el that made these function possible
	//==================================================================


	/// <summary>
	/// Constant used for internal operations.
	/// </summary>
	static const int kcbitUint = 32;

	/// <summary>
	/// Record used for internal building operations.
	/// </summary>
	struct Builder
	{
	private:
		/// <summary>
		/// Integer variable used for internal operations.
		/// </summary>
		/// <remarks>For a single UInt32, _iuLast is 0.</remarks>
		int _iuLast;

		/// <summary>
		/// UInt32 variable used for internal operations.
		/// </summary>
		/// <remarks>Used if _iuLast = 0.</remarks>
		UInt32 _uSmall;

		/// <summary>
		/// <see cref="TIntXLibUInt32Array" /> used for internal operations.
		/// </summary>
		/// <remarks>Used if _iuLast > 0.</remarks>
		vector<UInt32> _rgu;

	public:
		/// <summary>
		/// Used to create an Instance of <see cref="TOpHelper.TBuilder" />. for internal use only.
		/// </summary>
		Builder(const IntX &bn, int &sign)
		{
			int n, mask;

			_rgu = bn.digits;

			if (bn == 0) n = 0;
			else if (bn.negative) n = -1;
			else n = 1;

			mask = n >> (kcbitUint - 1);
			sign = (sign ^ mask) - mask;
			_iuLast = _rgu.size() - 1;
			_uSmall = _rgu[0];
			while (_iuLast > 0 && _rgu[_iuLast] == 0) --_iuLast;
		} // end constructor

		/// <summary>
		/// Function used for Internal operations.
		/// </summary>
		void GetApproxParts(int &exp, UInt64 &man)
		{
			int cuLeft, cbit;

			if (_iuLast == 0)
			{
				man = UInt64(_uSmall);
				exp = 0;
				return;
			} // end if

			cuLeft = _iuLast - 1;
			man = MakeUlong(_rgu[cuLeft + 1], _rgu[cuLeft]);
			exp = cuLeft * kcbitUint;
			cbit = CbitHighZero(_rgu[cuLeft + 1]);
			if (cuLeft > 0 && cbit > 0)
			{
				// Get 64 bits.
				man = (man << cbit) | (_rgu[cuLeft - 1] >> (kcbitUint - cbit));
				exp = exp - cbit;
			} // end if

		} // end function GetApproxParts

	}; // end struct Builder
	
	/// <summary>
	/// Record used for internal operations.
	/// </summary>
	/// <remarks>
	/// Both Record Fields are aligned at Zero Offsets.
	/// </remarks>
	struct DoubleUlong
	{
		union UULomg
		{
			/// <summary>
			/// Double variable used for internal operations.
			/// </summary>
			double dbl;

			/// <summary>
			/// UInt64 variable used for internal operations.
			/// </summary>
			UInt64 uu;
		};
	}; // end struct DoubleUlong
	
	/// <summary>
	/// Returns a Non-Negative Random <see cref="TIntX" /> object using Pcg Random.
	/// </summary>
	/// <returns>Random TIntX value.</returns>
	static IntX Random()
	{
		return Pcg::NextUInt32();
	} // end function Random

	/// <summary>
	/// Returns a Non-Negative Random <see cref="TIntX" /> object using Pcg Random within the specified Range. (Max not Included)
	/// </summary>
	/// <param name="Min">Minimum value.</param>
	/// <param name="Max">Maximum value (Max not Included)</param>
	/// <returns>Random TIntX value.</returns>
	static IntX RandomRange(const UInt32 Min, const UInt32 Max)
	{
		return Pcg::NextUInt32(Min, Max);
	} // end function RandomRange

	/// <summary>
	/// Returns a non negative big integer.
	/// </summary>
	/// <param name="value">value to get its absolute value.</param>
	/// <returns>Absolute number.</returns>
	static IntX AbsoluteValue(const IntX &value)
	{
		if (value.negative)
			return -(IntX)value;
		else
			return value;
	} // end function AbsoluteValue

	/// <summary>
	/// Returns a specified big integer raised to the power of 2.
	/// </summary>
	/// <param name="value">Number to get its square.</param>
	/// <returns>Squared number.</returns>
	static IntX Square(const IntX &value)
	{
		return  IntX::Pow(value, 2);
	} // end function Square

	/// <summary>
	/// Calculates Integer SquareRoot of <see cref="TIntX" /> object
	/// </summary>
	/// <param name="value">value to get Integer squareroot of.</param>
	/// <returns>Integer SquareRoot.</returns>
	/// <seealso href="http://www.dahuatu.com/RkWdPBx6W8.html">[IntegerSquareRoot Implementation]</seealso>
	static IntX IntegerSquareRoot(const IntX &value)
	{
		IntX a, b, mid, eight;

		if (value == 0) return 0;

		if (value == 1) return 1;

		a = 1;
		eight = 8;
		b = (value >> 5) + eight;

		while (b.CompareTo(a) >= 0)
		{
			mid = (a + b) >> 1;
			if ((mid * mid).CompareTo(value) > 0)
				b = (mid - 1);
			else
				a = (mid + 1);
		} // end while

		return a - 1;
	} // end function IntegerSquareRoot

	/// <summary>
	/// Returns a specified big integer holding the factorial of value.
	/// </summary>
	/// <param name="value">Number to get its factorial.</param>
	/// <returns>factorialed number.</returns>
	/// <exception cref="EArgumentException"><paramref name="value" /> is a negative value.</exception>
	static IntX Factorial(const IntX &value)
	{
		IntX i;

		// Exception
		if (value < 0)
			throw ArgumentException(Strings::NegativeFactorial + string(" [") + value.ToString() + string("]"));

		IntX result = 1;
		// using iterative approach
		// recursive approach is slower and causes much overhead. it is also limiting (stackoverflow)

		i = 1;
		while (i <= value)
		{
			result = result * i;
			++i;
		} // end while

		return result;
	} // end function 

	/// <summary>
	/// Returns the number of trailing 0-bits in x, starting at the least significant
	/// bit position.
	/// </summary>
	/// <param name="x">value to get number of trailing zero bits.</param>
	/// <returns>number of trailing 0-bits.</returns>
	/// <remarks>If x is 0, the result is undefined as per GCC Implementation.</remarks>
	static int __builtin_ctz(UInt32 x)
	{
		int	n;

		// This uses a binary search algorithm from Hacker's Delight.
		n = 1;
		if ((x & 0x0000FFFF) == 0)
		{
			n += 16;
			x >>= 16;
		} // end if

		if ((x & 0x000000FF) == 0)
		{
			n += 8;
			x >>= 8;
		} // end if

		if ((x & 0x0000000F) == 0)
		{
			n += 4;
			x >>= 4;
		} // end if

		if ((x & 0x00000003) == 0)
		{
			n += 2;
			x >>= 2;
		} // end if

		return n - int(x & 1);
	} // end function __builtin_ctz

	/// <summary>
	/// (Optimized GCD).
	/// Returns a specified big integer holding the GCD (Greatest common Divisor) of
	/// two big integers using Binary GCD (Stein's algorithm).
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>GCD number.</returns>
	/// <seealso href="http://lemire.me/blog/archives/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/">[GCD Implementation]</seealso>
	/// <seealso href="https://hbfs.wordpress.com/2013/12/10/the-speed-of-gcd/">[GCD Implementation Optimizations]</seealso>
	static IntX GCD(const IntX &intX1, const IntX &intX2)
	{
		IntX int1 = intX1, int2 = intX2;

		int shift;
		IntX temp;

		// check if int1 is negative and returns the absolute value of it.
		if (int1.negative)
			int1 = AbsoluteValue(int1);

		// check if int2 is negative and returns the absolute value of it.
		if (int2.negative)
			int2 = AbsoluteValue(int2);

		// simple cases (termination)

		if (int1 == int2) return int1;

		if (int1 == 0) return int2;

		if (int2 == 0) return int1;

		shift = __builtin_ctz(UInt32(int1 | int2));
		int1 = int1 >> __builtin_ctz(UInt32(int1));

		while (int2 != 0)
		{
			int2 = int2 >> __builtin_ctz(UInt32(int2));
			if (int1 > int2)
			{
				temp = int2;
				int2 = int1;
				int1 = temp;
			} // end if

			int2 = int2 - int1;
		} // end while

		return int1 << shift;
	} // end function GCD

	/// <summary>
	/// (LCM).
	/// Returns a specified big integer holding the LCM (Least Common Multiple) of
	/// two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>LCM number.</returns>
	/// <exception cref="EArgumentNilException"><paramref name="int1" /> is a null reference.</exception>
	/// <exception cref="EArgumentNilException"><paramref name="int2" /> is a null reference.</exception>
	static IntX LCM(const IntX &int1, const IntX &int2)
	{
		return IntX::AbsoluteValue(int1 * int2) / IntX::GCD(int1, int2);
	} // end function LCM

	/// <summary>
	/// Calculate Modular Inverse for two <see cref="TIntX" /> objects using Euclids Extended Algorithm.
	/// returns Zero if no Modular Inverse Exists for the Inputs
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Modular Inverse.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Modular_multiplicative_inverse">[Modular Inverse Explanation]</seealso>
	/// <seealso href="http://www.di-mgt.com.au/euclidean.html">[Modular Inverse Implementation]</seealso>
	static IntX InvMod(const IntX &int1, const IntX &int2)
	{
		IntX u1, u3, v1, v3, t1, t3, q, iter;

		/* Step X1. Initialize */
		u1 = 1;
		u3 = int1;
		v1 = 0;
		v3 = int2;

		/* Remember odd/even iterations */
		iter = 1;

		/* Step X2. Loop while v3 != 0 */
		while (v3 != 0)
		{
			/* Step X3. Divide and "Subtract" */
			q = u3 / v3;
			t3 = u3 % v3;
			t1 = u1 + q * v1;

			/* Swap */
			u1 = v1;
			v1 = t1;
			u3 = v3;
			v3 = t3;
			iter = -iter;
		} // end while

		/* Make sure u3 = gcd(u,v) == 1 */
		if (u3 != 1) // u3 is now holding the GCD Value
			return 0; /* Error: No inverse exists */

		/* Ensure a positive result */
		// result will hold the Modular Inverse (InvMod)
		if (iter < 0) return int2 - u1;

		return u1;
	} // end function InvMod

	/// <summary>
	/// Calculates Calculates Modular Exponentiation of <see cref="TIntX" /> object.
	/// </summary>
	/// <param name="value">value to compute ModPow of.</param>
	/// <param name="exponent">exponent to use.</param>
	/// <param name="modulus">modulus to use.</param>
	/// <returns>Computed value.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Modular_exponentiation">[Modular Exponentiation Explanation]</seealso>
	static IntX ModPow(const IntX &value, const IntX &exponent, const IntX &modulus)
	{
		IntX result = 1;
		IntX mValue = value % modulus;
		IntX mExponent = exponent;

		while (mExponent > 0)
		{
			if (mExponent.IsOdd()) result = (result * mValue) % modulus;

			mExponent = mExponent >> 1;
			mValue = (mValue * mValue) % modulus;
		} // end while

		return result;
	} // end function ModPow

	/// <summary>
	/// Calculates Bézoutsidentity for two <see cref="TIntX" /> objects using Euclids Extended Algorithm
	/// </summary>
	/// <param name="int1">first value.</param>
	/// <param name="int2">second value.</param>
	/// <param name="bezOne">first bezout value.</param>
	/// <param name="bezTwo">second bezout value.</param>
	/// <returns>GCD (Greatest Common Divisor) value.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Bézout's_identity">[Bézout's identity Explanation]</seealso>
	/// <seealso href="https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Pseudocode">[Bézout's identity Pseudocode using Extended Euclidean algorithm]</seealso>
	static IntX Bezoutsidentity(const IntX &int1, const IntX &int2, IntX &bezOne, IntX &bezTwo)
	{
		IntX s, t, r, old_s, old_t, old_r, quotient, prov;

		if (int1.negative)
			throw ArgumentNullException(Strings::BezoutNegativeNotAllowed + string(" int1"));

		if (int2.negative)
			throw ArgumentNullException(Strings::BezoutNegativeNotAllowed + string(" int2"));

		if (int1 == 0 || int2 == 0)
			throw ArgumentException(Strings::BezoutNegativeCantComputeZero);

		s = 0;
		t = 1;
		r = int2;
		old_s = 1;
		old_t = 0;
		old_r = int1;

		while (r != 0)
		{
			quotient = old_r / r;
			prov = r;
			r = old_r - quotient * prov;
			old_r = prov;
			prov = s;
			s = old_s - quotient * prov;
			old_s = prov;
			prov = t;
			t = old_t - quotient * prov;
			old_t = prov;
		} // end while

		if (old_r.negative) old_r = IntX::AbsoluteValue(old_r);

		bezOne = old_s; // old_s holds the first value of bezout identity
		bezTwo = old_t; // old_t holds the second value of bezout identity

		return old_r; // old_r holds the GCD Value
	} // end function Bezoutsidentity

	/// <summary>
	/// Checks if a <see cref="TIntX" /> object is Probably Prime using Miller–Rabin primality test.
	/// </summary>
	/// <param name="value">big integer to check primality.</param>
	/// <param name="Accuracy">Accuracy parameter `k´ of the Miller-Rabin algorithm. Default is 5. The execution time is proportional to the value of the accuracy parameter.</param>
	/// <returns>Boolean value.</returns>
	/// <seealso href="https://en.wikipedia.org/wiki/Miller–Rabin_primality_test">[Miller–Rabin primality test Explanation]</seealso>
	/// <seealso href="https://github.com/cslarsen/miller-rabin">[Miller–Rabin primality test Implementation in C]</seealso>
	static bool IsProbablyPrime(const IntX &value, const int Accuracy = 5)
	{
		return MillerRabin::IsProbablyPrimeMR(value, Accuracy);
	} // end function IsProbablyPrime

	/// <summary>
	/// The Max Between Two TIntX values.
	/// </summary>
	/// <param name="left">left value.</param>
	/// <param name="right">right value.</param>
	/// <returns>The Maximum TIntX value.</returns>
	static IntX Max(const IntX &left, const IntX &right)
	{
		if (left.CompareTo(right) < 0)
			return right;
		else
			return left;
	} // end function Max

	/// <summary>
	/// The Min Between Two TIntX values.
	/// </summary>
	/// <param name="left">left value.</param>
	/// <param name="right">right value.</param>
	/// <returns>The Minimum TIntX value.</returns>
	static IntX Min(const IntX &left, const IntX &right)
	{
		if (left.CompareTo(right) <= 0)
			return left;
		else
			return right;
	} // end function Min

	/// <summary>
	/// Function used for Internal operations.
	/// </summary>
	static UInt64 MakeUlong(const UInt32 uHi, const UInt32 uLo)
	{
		return (UInt64(uHi) << kcbitUint) | uLo;
	} // end function MakeUlong

	/// <summary>
	/// Function used for Internal operations.
	/// </summary>
	static int CbitHighZero(UInt32 u)
	{
		int cbit;

		if (u == 0) return 32;

		cbit = 0;
		if ((u & 0xFFFF0000) == 0)
		{
			cbit += 16;
			u <<= 16;
		} // end if

		if ((u & 0xFF000000) == 0)
		{
			cbit += 8;
			u <<= 8;
		} // end if

		if ((u & 0xF0000000) == 0)
		{
			cbit += 4;
			u <<= 4;
		} // end if

		if ((u & 0xC0000000) == 0)
		{
			cbit += 2;
			u <<= 2;
		} // end if

		if ((u & 0x80000000) == 0) cbit += 1;

		return cbit;
	} // end function CbitHighZero

	/// <summary>
	/// Function used for Internal operations.
	/// </summary>
	static int CbitHighZero(const UInt64 uu)
	{
		if ((uu & 0xFFFFFFFF00000000) == 0) return 32 + CbitHighZero(UInt32(uu));
		return CbitHighZero(UInt32(uu >> 32));
	} // end function CbitHighZero

	// Gotten from stack overflow
	static double logN(const double base, const double x)
	{
		return log(x) / log(base);
	} // end function logN

	/// <summary>
	/// Calculates Logarithm of a number <see cref="TIntX" /> object for a specified base.
	/// the largest power the base can be raised to that does not exceed the number.
	/// </summary>
	/// <param name="base">base.</param>
	/// <param name="value">number to get log of.</param>
	/// <returns>Log value.</returns>
	/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
	static double LogN(const double base, const IntX &value)
	{
		double tempD, tempTwo, pa, pb, pc, tempDouble;
		int c;
		long long b;
		UInt64 h, m, l, x; 
		
		const double constOne = 1.0, DoubleNaN = FP_NAN, DoublePositiveInfinity = FP_INFINITE, DoubleNegativeInfinity = -FP_INFINITE, ZeroPointZero = FP_ZERO;

		if (value.negative || base == constOne) return DoubleNaN;
		
		if (base == DoublePositiveInfinity)
		{
			if (value == 1) return ZeroPointZero;
			return DoubleNaN;
		} // end if

		if (base == ZeroPointZero && !(value == 1)) return DoubleNaN;
		
		if (value.digits.empty()) return DoubleNegativeInfinity;
		
		if (value <= Constants::MaxUInt64Value)
		{
			tempDouble = (UInt64)value;
			return logN(base, tempDouble);
		} // end if
		
		h = value.digits[value.digits.size() - 1];

		if (value.digits.size() > 1)
			m = value.digits[value.digits.size() - 2];
		else
			m = 0;

		if (value.digits.size() > 2)
			l = value.digits[value.digits.size() - 3];
		else
			l = 0;

		// measure the exact bit count
		c = CbitHighZero(UInt32(h));

		b = ((long long)value.digits.size() * 32) - c;

		// extract most significant bits
		x = (h << (32 + c)) | (m << c) | (l >> (32 - c));

		tempD = (double)x;
		tempTwo = 2;
		pa = logN(base, tempD);
		pc = logN(tempTwo, base);
		pb = (b - 64) / pc;
		
		return pa + pb;
	} // end function LogN

	/// <summary>
	/// Calculates Integer Logarithm of a number <see cref="TIntX" /> object for a specified base.
	/// the largest power the base can be raised to that does not exceed the number.
	/// </summary>
	/// <param name="base">base.</param>
	/// <param name="number">number to get Integer log of.</param>
	/// <returns>Integer Log.</returns>
	/// <seealso href="http://gist.github.com/dharmatech/409723">[IntegerLogN Implementation]</seealso>
	static IntX IntegerLogN(const IntX &base, const IntX &number)
	{
		IntX lo, b_lo, hi, mid, b_mid, b_hi;
		
		lo = 0;
		b_lo = 1;
		hi = 1;
		b_hi = base;

		while (b_hi < number)
		{
			lo = hi;
			b_lo = b_hi;
			hi = hi * 2;
			b_hi = b_hi * b_hi;
		} // end while

		while (hi - lo > 1)
		{
			mid = (lo + hi) / 2;
			b_mid = b_lo * (int)IntX::Pow(base, UInt32(mid - lo));
			if (number < b_mid)
			{
				hi = mid;
				b_hi = b_mid;
			} // end if

			if (number > b_mid)
			{
				lo = mid;
				b_lo = b_mid;
			} // end if

			if (number == b_mid) return mid;
		} // end while

		if (b_hi == number) return hi;
		return lo;
	} // end function IntegerLogN

	/// <summary>
	/// The base-10 logarithm of the value.
	/// </summary>
	/// <param name="value">The value.</param>
	/// <returns>The base-10 logarithm of the value.</returns>
	/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
	static double Log10(const IntX &value)
	{
		return OpHelper::LogN(10, value);
	} // end function Log10

	/// <summary>
	/// Calculates the natural logarithm of the value.
	/// </summary>
	/// <param name="value">The value.</param>
	/// <returns>The natural logarithm.</returns>
	/// <remarks> Source : Microsoft .NET Reference on GitHub </remarks>
	static double Ln(const IntX &value)
	{
		return OpHelper::LogN(Constants::EulersNumber, value);
	} // end function Ln
	
	/// <summary>
	/// Function used for Internal operations.
	/// </summary>
	/// <param name="mdbl">
	/// internal variable
	/// </param>
	/// <param name="sign">
	/// variable used to indicate sign
	/// </param>
	/// <param name="exp">
	/// internal variable
	/// </param>
	/// <param name="man">
	/// internal variable
	/// </param>
	/// <param name="fFinite">
	/// internal variable
	/// </param>
	/// <remarks>
	/// Source : Microsoft .NET Reference on GitHub
	/// </remarks>
	static void GetDoubleParts(const double mdbl, int &sign, int &exp, UInt64 &man, bool &fFinite)
	{
		DoubleUlong::UULomg	du;
		
		du.uu = 0;
		du.dbl = mdbl;

		sign = 1 - (int(du.uu >> 62) & 2);

		man = du.uu & 0x000FFFFFFFFFFFFF;
		exp = int(du.uu >> 52) & 0x7FF;
		if (exp == 0)
		{
			// Denormalized number.
			fFinite = true;
			if (man != 0)
				exp = -1074;
		} // end if
		else
		{
			if (exp == 0x7FF)
			{
				// NaN or Inifite.
				fFinite = false;
				exp = Constants::MaxIntValue;
			} // end if
			else
			{
				fFinite = true;
				man = man | 0x0010000000000000;
				exp = exp - 1075;
			} // end else
		} // end else
	} // end function GetDoubleParts

	/// <summary>
	/// Function used for Internal operations.
	/// </summary>
	/// <param name="sign">
	/// variable indicating sign
	/// </param>
	/// <param name="exp">
	/// variable used for internal operations
	/// </param>
	/// <param name="man">
	/// variable used for internal operations
	/// </param>
	/// <remarks>
	/// Source : Microsoft .NET Reference on GitHub
	/// </remarks>
	static double GetDoubleFromParts(int sign, int exp, UInt64 man)
	{
		DoubleUlong::UULomg	du;
		int cbitShift;
		
		du.dbl = 0;

		if (man == 0) du.uu = 0;
		else
		{
			// Normalize so that $0010 0000 0000 0000 is the highest bit set.
			cbitShift = CbitHighZero(man) - 11;
			if (cbitShift < 0) man = man >> -cbitShift;
			else man = man << cbitShift;
				
			exp = exp - cbitShift;
	
			// Move the point to just behind the leading 1: $001.0 0000 0000 0000
			// (52 bits) and skew the exponent (by $3FF == 1023).
			exp = exp + 1075;

			if (exp >= 0x7FF)
				// Infinity.
				du.uu = 0x7FF0000000000000;
			else if (exp <= 0)
			{
				// Denormalized.
				--exp;

				if (exp < -52)
					// Underflow to zero.
					du.uu = 0;
				else
					du.uu = man >> -exp;
			} // end else if
			else
				// Mask off the implicit high bit.
				du.uu = (man & 0x000FFFFFFFFFFFFF) | (UInt64(exp) << 52);
		} // end else

		if (sign < 0)
			du.uu = du.uu | 0x8000000000000000;

		return du.dbl;
	} // end function GetDoubleFromParts

	/// <summary>
	/// function used for Internal operations
	/// </summary>
	/// <param name="value">
	/// value to process
	/// </param>
	/// <param name="digits">
	/// digits array to process
	/// </param>
	/// <param name="newInt">
	/// output result
	/// </param>
	/// <remarks>
	/// Source : Microsoft .NET Reference on GitHub
	/// </remarks>
	static void SetDigitsFromDouble(const double value, vector<UInt32> &digits, IntX &newInt)
	{
		int sign, exp, kcbitUlong, kcbitUint, cu, cbit;
		UInt64 man;
		bool fFinite, tempSign;

		kcbitUint = 32;
		kcbitUlong = 64;
		OpHelper::GetDoubleParts(value, sign, exp, man, fFinite);

		if (man == 0)
		{
			newInt = 0;
			return;
		} // end if

		if (exp <= 0)
		{
			if (exp <= -kcbitUlong)
			{
				newInt = 0;
				return;
			} // end if

			newInt = man >> -exp;
			if (sign < 0) newInt.negative = true;
		} // end if
		else if (exp <= 11) 
		{
			newInt = man << exp;
			if (sign < 0) newInt.negative = true;
		} // end else if
		else
		{
			// Overflow into at least 3 uints.
			// Move the leading 1 to the high bit.
			man = man << 11;
			exp = exp - 11;

			// Compute cu and cbit so that exp = 32 * cu - cbit and 0 <= cbit < 32.
			cu = ((exp - 1) / kcbitUint) + 1;
			cbit = cu * kcbitUint - exp;

			// Populate the uints.
			digits.resize(cu + 2);
			digits[cu + 1] = UInt32((man >> (cbit + kcbitUint)));
			digits[cu] = UInt32(man >> cbit);

			if (cbit > 0) digits[cu - 1] = UInt32(man) << (kcbitUint - cbit);
			if (sign < 0) tempSign = true;
			else tempSign = false;

			newInt = IntX(digits, tempSign);
			newInt.negative = tempSign;
		} // end else
	} // end function SetDigitsFromDouble

	//==================================================================
	//  The End
	//==================================================================


	/// <summary>
	/// Compares 2 <see cref="IntX" /> objects.
	/// Returns "-2" if any argument is null, "-1" if <paramref name="int1" /> &lt; <paramref name="int2" />,
	/// "0" if equal and "1" if &gt;.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <param name="throwNullException">Raises or not <see cref="NullReferenceException" />.</param>
	/// <returns>Comparsion result.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="int1" /> or <paramref name="int2" /> is a null reference and <paramref name="throwNullException" /> is set to true.</exception>
	static int Cmp(const IntX &int1, const IntX &int2, const bool throwNullException)
	{
		if (int1.length == 0)
		{
			if (int2.length == 0) return 0;
			else if (int2.negative) return 1;
			return -1;
		} // end if
		else if (int2.length == 0)
		{
			if (int1.negative) return -1;
			return 1;
		} // end else if
		
		// Compare sign
		if (int1.negative && !int2.negative) return -1;
		if (!int1.negative && int2.negative) return 1;

		// Compare presentation
		return DigitOpHelper::Cmp(&int1.digits[0], int1.length, &int2.digits[0], int2.length) * (int1.negative ? -1 : 1);
	} // end function Cmp

	/// <summary>
	/// Compares <see cref="IntX" /> object to int.
	/// Returns "-1" if <paramref name="int1" /> &lt; <paramref name="int2" />, "0" if equal and "1" if &gt;.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second integer.</param>
	/// <returns>Comparsion result.</returns>
	static int Cmp(const IntX &int1, const int int2)
	{
		// Special processing for zero
		if (int2 == 0) return int1.length == 0 ? 0 : (int1.negative ? -1 : 1);
		if (int1.length == 0) return int2 > 0 ? -1 : 1;

		// Compare presentation
		if (int1.length > 1) return int1.negative ? -1 : 1;
		UInt32 digit2;
		bool negative2;
		DigitHelper::ToUInt32WithSign(int2, digit2, negative2);

		// Compare sign
		if (int1.negative && !negative2) return -1;
		if (!int1.negative && negative2) return 1;

		return int1.digits[0] == digit2 ? 0 : ((int1.digits[0] < digit2) ^ negative2 ? -1 : 1);
	} // end function Cmp
  
	/// <summary>
	/// Compares <see cref="IntX" /> object to unsigned int.
	/// Returns "-1" if <paramref name="int1" /> &lt; <paramref name="int2" />, "0" if equal and "1" if &gt;.
	/// For internal use.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second unsigned integer.</param>
	/// <returns>Comparsion result.</returns>
	static int Cmp(const IntX &int1, const UInt32 int2)
	{
		// Special processing for zero
		if (int2 == 0) return int1.length == 0 ? 0 : (int1.negative ? -1 : 1);
		if (int1.length == 0) return -1;

		// Compare presentation
		if (int1.negative) return -1;
		if (int1.length > 1) return 1;
		return int1.digits[0] == int2 ? 0 : (int1.digits[0] < int2 ? -1 : 1);
	} // end function Cmp

	/// <summary>
	/// Shifts <see cref="IntX" /> object.
	/// Determines which operation to use basing on shift sign.
	/// </summary>
	/// <param name="intX">Big integer.</param>
	/// <param name="shift">Bits count to shift.</param>
	/// <param name="toLeft">If true the shifting to the left.</param>
	/// <returns>Bitwise shift operation result.</returns>
	/// <exception cref="ArgumentNullException"><paramref name="intX" /> is a null reference.</exception>
	static IntX Sh(const IntX &intX, const long long shift, bool toLeft)
	{
		// Zero can't be shifted
		if (intX.length == 0) return IntX();

		// Can't shift on zero value
		if (shift == 0) return IntX(intX);

		// Determine real bits count and direction
		UInt64 bitCount;
		bool negativeShift;
		DigitHelper::ToUInt64WithSign(shift, bitCount, negativeShift);
		toLeft ^= negativeShift;

		// Get position of the most significant bit in intX and amount of bits in intX
		int msb = Bits::Msb(intX.digits[intX.length - 1]);
		UInt64 intXBitCount = (UInt64)(intX.length - 1) * Constants::DigitBitCount + (UInt64)msb + 1UL;

		// If shifting to the right and shift is too big then return zero
		if (!toLeft && bitCount >= intXBitCount) return IntX();

		// Calculate new bit count
		UInt64 newBitCount = toLeft ? intXBitCount + bitCount : intXBitCount - bitCount;

		// If shifting to the left and shift is too big to fit in big integer, throw an exception
		if (toLeft && newBitCount > Constants::MaxBitCount)
		{
			throw ArgumentException(Strings::IntegerTooBig);
		} // end if

		  // Get exact length of new big integer (no normalize is ever needed here).
		  // Create new big integer with given length
		UInt32 newLength = (UInt32)(newBitCount / Constants::DigitBitCount + (newBitCount % Constants::DigitBitCount == 0 ? 0UL : 1UL));
		IntX newInt = IntX(newLength, intX.negative);

		// Get full and small shift values
		UInt32 fullDigits = (UInt32)(bitCount / Constants::DigitBitCount);
		int smallShift = (int)(bitCount % Constants::DigitBitCount);

		// We can just copy (no shift) if small shift is zero
		if (smallShift == 0)
		{
			if (toLeft)
			{
				memmove(&newInt.digits[fullDigits], &intX.digits[0], intX.length * sizeof(UInt32));
			} // end if
			else
			{
				memmove(&newInt.digits[0], &intX.digits[fullDigits], newLength * sizeof(UInt32));
			} // end else
		} // end if
		else
		{
			// Do copy with real shift in the needed direction
			if (toLeft)
			{
				DigitOpHelper::Shr(&intX.digits[0], 0, intX.length, &newInt.digits[0], fullDigits + 1, Constants::DigitBitCount - smallShift);
			} // end if
			else
			{
				// If new result length is smaller then original length we shouldn't lose any digits
				if (newLength < (intX.length - fullDigits))
				{
					newLength++;
				} // end if

				DigitOpHelper::Shr(&intX.digits[0], fullDigits, newLength, &newInt.digits[0], 0, smallShift);
			} // end else
		} // end else

		return newInt;
	} // end function Sh
  
	/// <summary>
	/// Performs bitwise OR for two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	static IntX BitwiseOr(const IntX &int1, const IntX &int2)
	{
		 // Process zero values in special way
		if (int1.length == 0)
		{
			return IntX(int2);
		} // end if
		if (int2.length == 0)
		{
			return IntX(int1);
		} // end if

		// Determine big int with lower length
		IntX smallerInt;
		IntX biggerInt;
		GetMinMaxLengthObjects(int1, int2, smallerInt, biggerInt);

		// Create new big int object of needed length
		IntX newInt = IntX(biggerInt.length, int1.negative | int2.negative);

		// Do actual operation
		DigitOpHelper::BitwiseOr(
			&biggerInt.digits[0],
			biggerInt.length,
			&smallerInt.digits[0],
			smallerInt.length,
			&newInt.digits[0]);

		// Normalization may be needed
		newInt.TryNormalize();

		return newInt;
	} // end function BitwiseOr

	/// <summary>
	/// Performs bitwise AND for two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	static IntX BitwiseAnd(const IntX &int1, const IntX &int2)
	{
		// Process zero values in special way
		if (int1.length == 0 || int2.length == 0)
		{
			return IntX();
		} // end if

		// Determine big int with lower length
		IntX smallerInt;
		IntX biggerInt;
		GetMinMaxLengthObjects(int1, int2, smallerInt, biggerInt);

		// Create new big int object of needed length
		IntX newInt = IntX(smallerInt.length, int1.negative & int2.negative);

		// Do actual operation
		newInt.length = DigitOpHelper::BitwiseAnd(
			&biggerInt.digits[0],
			&smallerInt.digits[0],
			smallerInt.length,
			&newInt.digits[0]);

		// Normalization may be needed
		newInt.TryNormalize();

		return newInt;
	} // end function BitwiseAnd

	/// <summary>
	/// Performs bitwise XOR for two big integers.
	/// </summary>
	/// <param name="int1">First big integer.</param>
	/// <param name="int2">Second big integer.</param>
	/// <returns>Resulting big integer.</returns>
	static IntX ExclusiveOr(const IntX &int1, const IntX &int2)
	{
		// Process zero values in special way
		if (int1.length == 0)
		{
			return IntX(int2);
		} // end if
		if (int2.length == 0)
		{
			return IntX(int1);
		} // end if

		  // Determine big int with lower length
		IntX smallerInt;
		IntX biggerInt;
		GetMinMaxLengthObjects(int1, int2, smallerInt, biggerInt);

		// Create new big int object of needed length
		IntX newInt = IntX(biggerInt.length, int1.negative ^ int2.negative);

		// Do actual operation
		newInt.length = DigitOpHelper::ExclusiveOr(
			&biggerInt.digits[0],
			biggerInt.length,
			&smallerInt.digits[0],
			smallerInt.length,
			&newInt.digits[0]);

		// Normalization may be needed
		newInt.TryNormalize();
		
		return newInt;
	} // end function ExclusiveOr

	/// <summary>
	/// Performs bitwise NOT for big integer.
	/// </summary>
	/// <param name="value">Big integer.</param>
	/// <returns>Resulting big integer.</returns>
	static IntX OnesComplement(const IntX &value)
	{
		// Process zero values in special way
		if (value.length == 0)
		{
			return IntX();
		} // end if

		// Create new big int object of needed length
		IntX newInt = IntX(value.length, !value.negative);

		// Do actual operation
		newInt.length = DigitOpHelper::OnesComplement(
			&value.digits[0],
			value.length,
			&newInt.digits[0]);

		// Normalization may be needed
		newInt.TryNormalize();

		return newInt;
	} // end function OnesComplement

}; // end class OpHelper

#endif // !OPHELPER_H