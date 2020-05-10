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
#include "extdll.h"
#include "util.h"

#include "WeaponsConst.h"

const char* ItemInfoFlagToString( const ItemInfoFlag flag )
{
	switch( flag )
	{
	case ITEM_FLAG_NONE:				return "NONE";
	case ITEM_FLAG_SELECTONEMPTY:		return "SELECTONEMPTY";
	case ITEM_FLAG_NOAUTORELOAD:		return "NOAUTORELOAD";
	case ITEM_FLAG_NOAUTOSWITCHEMPTY:	return "NOAUTOSWITCHEMPTY";
	case ITEM_FLAG_LIMITINWORLD:		return "LIMITINWORLD";
	case ITEM_FLAG_EXHAUSTIBLE:			return "EXHAUSTIBLE";
	}

	ASSERTSZ( false, "ItemInfoFlagToString: unknown constant!" );

	return "NONE";
}

ItemInfoFlag StringToItemInfoFlag( const char* const pszString )
{
	if( !pszString )
		return ITEM_FLAG_NONE;

	if( stricmp( pszString, "NONE" ) == 0 )
		return ITEM_FLAG_NONE;

	if( stricmp( pszString, "SELECTONEMPTY" ) == 0 )
		return ITEM_FLAG_SELECTONEMPTY;

	if( stricmp( pszString, "NOAUTORELOAD" ) == 0 )
		return ITEM_FLAG_NOAUTORELOAD;

	if( stricmp( pszString, "NOAUTOSWITCHEMPTY" ) == 0 )
		return ITEM_FLAG_NOAUTOSWITCHEMPTY;

	if( stricmp( pszString, "LIMITINWORLD" ) == 0 )
		return ITEM_FLAG_LIMITINWORLD;

	if( stricmp( pszString, "EXHAUSTIBLE" ) == 0 )
		return ITEM_FLAG_EXHAUSTIBLE;

	return ITEM_FLAG_NONE;
}