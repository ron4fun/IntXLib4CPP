#pragma once

#ifndef MILLERRABIN_H
#define MILLERRABIN_H

#include "IntX.h"

class MillerRabin
{
public:
	static bool IsProbablyPrimeMR(const IntX &n, const int k = 5)
	{
		int s, i, r;
		IntX m, d, a, x;
		bool Redo;

		Redo = false;
		// Must have ODD n greater than THREE
		if (n == 2 || n == 3) return true;
		
		else if (n <= 1 || !n.IsOdd()) return false;

		// Write n-1 as d*2^s by factoring powers of 2 from n-1
		s = 0;
		m = n - 1;
		while (!m.IsOdd())
		{
			++s;
			m = m >> 1;
		} // end while

		d = (n - 1) / (1 << s);

		i = 0;
		while (i < k)
		{
			a = IntX::RandomRange(2, UInt32(n - 2));
			x = IntX::ModPow(a, d, n);
			if (x == 1 || x == n - 1)
			{
				++i;
				continue;
			} // end if

			r = 1;
			while (r <= (s - 1))
			{
				x = IntX::ModPow(x, 2, n);
				if (x == 1) return false;
				
				else if (x == n - 1)
				{
					Redo = true;
					break;
				} // end else if
			
				++r;
			} // end while

			if (!Redo) return false;
			else
			{
				++i;
				continue;
			} // end else

		} // end while
		
		// n is *probably* prime
		return true;
	} // end function IsProbablyPrimeMR

}; // end class MillerRabin

#endif // !MILLERRABIN_H
