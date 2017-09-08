#pragma once

#ifndef SRINGCONVERTERMANAGER_H
#define SRINGCONVERTERMANAGER_H

#include "ClassicStringConverter.h"
#include "FastStringConverter.h"
#include "IStringConverter.h"
#include "IntX.h"

// Used to retrieve needed ToString converter.
class StringConvertManager
{
private:
	
	// Classic converter instance.
	static ClassicStringConverter _ClassicStringConverter;

	// Fast converter instance.
	static FastStringConverter _FastStringConverter;

	// Create new pow2 converter instance
	static Pow2StringConverter pow2StringConverter;

public:
	/*
	/// <summary>
	/// Returns ToString converter instance for given ToString mode.
	/// </summary>
	/// <param name="mode">ToString mode.</param>
	/// <returns>Converter instance.</returns>
	/// <exception cref="ArgumentOutOfRangeException"><paramref name="mode" /> is out of range.</exception>
	*/
	static IStringConverter* GetStringConverter(ToStringMode mode)
	{
		switch (mode)
		{
		case ToStringMode::tsmFast:
			return &_FastStringConverter;
		case ToStringMode::tsmClassic:
			return &_ClassicStringConverter;
		default:
			throw ArgumentOutOfRangeException("mode");
		} // end switch
	} // end function GetStringConverter

}; // end class StringConvertManager

// Create new pow2 converter instance
Pow2StringConverter StringConvertManager::pow2StringConverter = Pow2StringConverter();

// Classic converter instance.
ClassicStringConverter StringConvertManager::_ClassicStringConverter = ClassicStringConverter(StringConvertManager::pow2StringConverter);

// Fast converter instance.
FastStringConverter StringConvertManager::_FastStringConverter = FastStringConverter(
																StringConvertManager::pow2StringConverter,
																StringConvertManager::_ClassicStringConverter);

#endif // !SRINGCONVERTERMANAGER_H
