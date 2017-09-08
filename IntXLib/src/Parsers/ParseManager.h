#pragma once

#ifndef PARSEMANAGER_H
#define PARSEMANAGER_H

#include "IParser.h"
#include "Pow2Parser.h"
#include "ClassicParser.h"
#include "FastParser.h"
#include "IntX.h"

class  ParseManager : public IParser
{
private:
	static ClassicParser _ClassicParser;
	static FastParser _FastParser;
	
	// Create new pow2 parser instance
	static Pow2Parser pow2Parser;

public:
	/*
	/// <summary>
	/// Returns parser instance for given parse mode.
	/// </summary>
	/// <param name="mode">Parse mode.</param>
	/// <returns>Parser instance.</returns>
	/// <exception cref="ArgumentOutOfRangeException"><paramref name="mode" /> is out of range.</exception>
	*/
	static IParser* GetParser(ParseMode mode)
	{
		switch (mode)
		{
		case ParseMode::pmFast:
			return &_FastParser;
		case ParseMode::pmClassic:
			return &_ClassicParser;
		default:
			throw ArgumentOutOfRangeException("mode");
		} // end switch
	} // end function GetParser

	/*
	/// <summary>
	/// Returns current parser instance.
	/// </summary>
	/// <returns>Current parser instance.</returns>
	*/
	static IParser* GetCurrentParser()
	{
		return GetParser(IntX::getGlobalSettings()->getParseMode());
	} // end function GetCurrentParser

}; // end class

Pow2Parser ParseManager::pow2Parser = Pow2Parser();
ClassicParser ParseManager::_ClassicParser = ClassicParser(ParseManager::pow2Parser);
FastParser ParseManager::_FastParser = FastParser(ParseManager::pow2Parser, ParseManager::_ClassicParser);

#endif // !PARSEMANAGER_H