#pragma once

#ifndef STRREPHELPER_H
#define STRREPHELPER_H

// data types
typedef unsigned long long UInt64;
typedef unsigned int UInt32;

#include "Utils.h"
#include "Strings.h"
#include "Dictionary.h"
#include <string>


class StrRepHelper
{
public:
	
	/// <summary>
	/// Returns digit for given char.
	/// </summary>
	/// <param name="charToDigits">Char->digit dictionary.</param>
	/// <param name="ch">Char which represents big integer digit.</param>
	/// <param name="numberBase">String representation number base.</param>
	/// <returns>Digit.</returns>
	/// <exception cref="FormatException"><paramref name="ch" /> is not in valid format.</exception>
	static UInt32 GetDigit(const Dictionary &charToDigits, const char ch, const unsigned int numberBase)
	{
		if (charToDigits.size() == 0)
		{
			throw ArgumentNullException("charToDigits");
		} // end if

		UInt32 digit;

		try
		{
			// Try to identify this digit
			digit = charToDigits.at(ch);
		} // end try
		catch (const std::exception &e)
		{
			throw FormatException(string(Strings::ParseInvalidChar) + e.what());
		} // end  catch
	
		if (digit >= numberBase)
		{
			throw FormatException(Strings::ParseTooBigDigit);
		} // end if

		return digit;
	} // end function GetDigit

	/// <summary>
	/// Verfies string alphabet provider by user for validity.
	/// </summary>
	/// <param name="alphabet">Alphabet.</param>
	/// <param name="numberBase">String representation number base.</param>
	static void AssertAlphabet(const string &alphabet, const UInt32 numberBase)
	{
		if (alphabet.length() == 0)
		{
			throw ArgumentNullException("alphabet");
		} // end if

		// Ensure that alphabet has enough characters to represent numbers in given base
		if (alphabet.length() < numberBase)
		{
			throw ArgumentException(Strings::AlphabetTooSmall);
		} // end if

		// Ensure that all the characters in alphabet are unique
		string sortedChars = string(alphabet);
		
		// insertion sort
		// loop over the elements of the array
		char insert;
		for (UInt32 next = 1; next < sortedChars.length(); ++next)
		{
			insert = sortedChars[next]; // store the value in the current element
			int moveItem = next; // initialize location to place element
									// search for the location in which to put the current element
			while ((moveItem > 0) && (sortedChars[moveItem - 1] > insert))
			{
				// shift element one slot to the right
				sortedChars[moveItem] = sortedChars[moveItem - 1];
				moveItem--;
			} // end while
			sortedChars[moveItem] = insert; // place inserted element into the array
		} // end for

		//qsort()
		//Array.Sort(sortedChars);
		for (UInt32 i = 0; i < alphabet.length(); i++)
		{
			if (i > 0 && sortedChars[i] == sortedChars[i - 1])
			{
				throw ArgumentException(Strings::AlphabetRepeatingChars);
			} // end if
		} // end for
	} // end function AssertAlphabet

	/// <summary>
	/// Generates char->digit dictionary from alphabet.
	/// </summary>
	/// <param name="alphabet">Alphabet.</param>
	/// <param name="numberBase">String representation number base.</param>
	/// <returns>Char->digit dictionary.</returns>
	static Dictionary CharDictionaryFromAlphabet(const string &alphabet, const UInt32 numberBase)
	{
		AssertAlphabet(alphabet, numberBase);
		Dictionary charToDigits; // int[(int)numberBase];

		for (UInt32 i = 0; i < numberBase; i++)
		{
			charToDigits[alphabet[i]] = (UInt32)i;
		} // end for

		return charToDigits;
	} // end function CharDictionaryFromAlphabet
	
}; // end class StrRepHelper


#endif // !STRREPHELPER_H