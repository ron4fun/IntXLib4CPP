IntX
====

 **`IntX`** is a C++11 port of [IntX](https://github.com/devoyster/IntXLib) arbitrary precision Integer library with speed, about **`O(N * log N)`** multiplication/division algorithms implementation. It provides all the basic arithmetic operations on Integers, comparing, bitwise shifting etc. It also allows parsing numbers in different bases and converting them to string, also in any base. The advantage of this library is its fast multiplication, division and from base/to base conversion algorithms. all the fast versions of the algorithms are based on fast multiplication of big Integers using [Fast Hartley Transform](http://en.wikipedia.org/wiki/Discrete_Hartley_transform) which runs for **`O(N * log N * log log N)`** time instead of classic **`O(N^2)`**.
  

Code Example
------------

Here is a sample of code which uses `IntX` to calculate 42 in power 1048576 (which is 2^20 (1 shl 20)):
    

	#include <iostream>
	#include <Windows.h>
	#include "Settings\IntXGlobalSettings.h"
	#include "IntX.h"
	
	void Calc()
	{
		uint32_t valA, valB;
		double Delta;
	
		valA = GetTickCount();
		IntX::Pow(42, 1048576);
		valB = GetTickCount();
		Delta = (valB - valA) / 1000;
	
		cout << Delta << endl;
	}

	int main()
	{
		Calc();

		system("PAUSE");
	
		TIntX::getGlobalSettings()->setMultiplyMode(MultiplyMode::mmClassic);
	
		Calc();
		
	    return 0;
	} // end main


    First 'Calc()' call uses fast multiplication implementation (which is default), 
    second, classic one. On my machine (Windows 10, Intel Core i5 2.20 GHz, 
    8 GB RAM), Compiled with 64 bits, first call took 0 seconds while the second one 
    took 13 seconds.Resulting number has 1,702,101 digits.

**Some other functions implemented internally by me are**

  
      IntegerSquareRoot (Integer SquareRoot) 
      Square 
      GCD (Greatest Common Divisor (HCF)) 
      LCM (Least Common Multiple)
      AbsoluteValue (Get Absolute Value of a Negative TIntX)
      Bézouts Identity
      InvMod (Modular Inverse)
      Factorial
      IntegerLogN (base, number) (Gets IntegerLog of a number using a specified base)
	  Ln (The natural logarithm)
	  Log10 (The base-10 logarithm)
	  LogN (Logarithm of a number for a specified base)
      Random (Now Uses PcgRandom Instead of Mersemme Twister)
      Modular Exponentiation (ModPow)
      IsProbablyPrime (based on Miller Rabin Primality Test)

As you can see, `IntX` implements all the standard arithmetic operators using `operator overloading` so its usage is transparent for developers, like if you're working with usual Integers.

FHT and Calculations Precision
------------------------------

Internally `IntX` library operates with floating-point numbers when multiplication using FHT (Fast Hartley Transform) is performed so at some point it stops working correctly and loses precision. Luckily, this unpleasant side-effects effects starts to appear when Integer size is about 2^28 bytes i.e. for really huge Integers. Anyway, to catch such errors some code was added, FHT multiplication result validity check into code -- it takes N last digits of each big Integer, multiplies them using classic approach and then compares last N digits of classic result with last N digits of FHT result (so it's kind of  a simplified CRC check). If any inconsistency is found, then an 
`FhtMultiplicationException` is thrown; this check can be disabled using global settings.

Internal Representation and ToString() Performance
--------------------------------------------------

   For a really huge Integer numbers (like 42 in power 1048576 above) `ToString()` 
 call can take quite some time to execute. This is because, internally `IntX` big 
 Integers are stored as `2^32`-base number in `UInt32` array and to generate decimal 
 string output it should be converted from `2^32` base to decimal base. Such digits 
 storage approach was chosen intentionally -- it makes `ToString()` slower but uses 
 memory efficiently and makes primitive operations on digits faster than power of 
 10-base storage (which would make `ToString()` work faster) and 
 usually computations are used more often than `ToString()`.

**Tested Enviroments:**
     
    Visual Studio 2015.


###License

This "Software" is Licensed Under  **`MIT License (MIT)`** .
    

Conclusion
--------------------------------------------------

    Special Thanks to first of all, (Andriy Kozachuk) for creating the Original CSharp
	version and Xor-el for various support offered.
