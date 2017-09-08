#pragma once

#ifndef BITS_H
#define BITS_H

// Contains helping methods to with with bits in dword (<see cref="UInt32" />).
class Bits
{
public:
	
	/// <summary>
	/// Returns number of leading zero bits in int.
	/// </summary>
	/// <param name="x">Int value.</param>
	/// <returns>Number of leading zero bits.</returns>
	static int Nlz(const unsigned int  value)
	{
		unsigned int x = value;

		if (x == 0) return 32;

		int n = 1;
		if ((x >> 16) == 0) { n += 16; x <<= 16; }
		if ((x >> 24) == 0) { n += 8; x <<= 8; }
		if ((x >> 28) == 0) { n += 4; x <<= 4; }
		if ((x >> 30) == 0) { n += 2; x <<= 2; }
		return n - (int)(x >> 31);
	} // end function Nlz

	/// <summary>
	/// Counts position of the most significant bit in int.
	/// Can also be used as Floor(Log2(<paramref name="x" />)).
	/// </summary>
	/// <param name="x">Int value.</param>
	/// <returns>Position of the most significant one bit (-1 if all zeroes).</returns>
	static int Msb(const unsigned int x)
	{
		return 31 - Nlz(x);
	} // end function Msb

	/// <summary>
	/// Ceil(Log2(<paramref name="x" />)).
	/// </summary>
	/// <param name="x">Int value.</param>
	/// <returns>Ceil of the Log2.</returns>
	static int CeilLog2(const unsigned int x)
	{
		int msb = Msb(x);
		if (x != 1U << msb)
		{
			++msb;
		}
		return msb;
	} // end function CeilLog2

}; // end class Bits

#endif