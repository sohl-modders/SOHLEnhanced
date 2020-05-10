/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef GAME_SERVER_MAPCYCLE_H
#define GAME_SERVER_MAPCYCLE_H

/**
*	Represents a map cycle.
*/
class CMapCycle
{
public:
	static const size_t MAX_RULE_BUFFER = 1024;

	struct Item_t
	{
		Item_t* next;

		char mapname[ cchMapNameMost ];
		int  minplayers, maxplayers;
		char rulebuffer[ MAX_RULE_BUFFER ];
	};

public:
	/**
	*	Constructs an empty map cycle.
	*/
	CMapCycle() = default;

	/**
	*	Gets the name of the map cycle that was loaded, if any.
	*/
	const char* GetFileName() const { return m_szFileName; }

	/**
	*	Gets the list of items.
	*/
	Item_t* GetItems() const { return items; }

	/**
	*	Gets the next item in the list.
	*/
	Item_t* GetNextItem() const { return next_item; }

	/**
	*	Sets the next item in the list.
	*/
	void SetNextItem( Item_t* pItem )
	{
		next_item = pItem;
	}

	/*
	*	Parses mapcycle.txt file into mapcycle_t structure.
	*	@param pszFileName Name of the map cycle file to load.
	*	@return Whether the file was successfully loaded or not.
	*/
	bool LoadMapCycleFile( const char* const pszFileName );

	/*
	*	Clean up memory used by mapcycle when switching it.
	*/
	void Clear();

private:
	char m_szFileName[ MAX_PATH ] = {};

	Item_t *items = nullptr;
	Item_t *next_item = nullptr;

private:
	CMapCycle( const CMapCycle& ) = delete;
	CMapCycle& operator=( const CMapCycle& ) = delete;
};

/*
*	Parse commands/key value pairs to issue right after map xxx command is issued on server
*	level transition
*	@param pszToken Token to parse.
*	@param[ out ] pszCommand Command that was parsed out of the token.
*/
void ExtractCommandString( const char* pszToken, char* pszCommand );

/**
*	The map cycle.
*/
extern CMapCycle g_MapCycle;

#endif //GAME_SERVER_MAPCYCLE_H
