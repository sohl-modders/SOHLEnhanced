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

#include "MapCycle.h"

CMapCycle g_MapCycle;

bool CMapCycle::LoadMapCycleFile( const char* pszFileName )
{
	strncpy( m_szFileName, pszFileName, sizeof( m_szFileName ) );
	m_szFileName[ sizeof( m_szFileName ) - 1 ] = '\0';

	char szBuffer[ MAX_RULE_BUFFER ];
	char szMap[ cchMapNameMost ];
	int length;
	char *aFileList = ( char* ) LOAD_FILE_FOR_ME( pszFileName, &length );

	const char* pFileList = aFileList;
	bool hasbuffer;
	Item_t *item, *newlist = NULL, *next;

	if( pFileList && length )
	{
		// the first map name in the file becomes the default
		while( 1 )
		{
			hasbuffer = false;
			memset( szBuffer, 0, MAX_RULE_BUFFER );

			pFileList = COM_Parse( pFileList );
			if( strlen( com_token ) <= 0 )
				break;

			strcpy( szMap, com_token );

			// Any more tokens on this line?
			if( COM_TokenWaiting( pFileList ) )
			{
				pFileList = COM_Parse( pFileList );
				if( strlen( com_token ) > 0 )
				{
					hasbuffer = true;
					strcpy( szBuffer, com_token );
				}
			}

			// Check map
			if( IS_MAP_VALID( szMap ) )
			{
				// Create entry
				char *s;

				item = new Item_t;

				strcpy( item->mapname, szMap );

				item->minplayers = 0;
				item->maxplayers = 0;

				memset( item->rulebuffer, 0, MAX_RULE_BUFFER );

				if( hasbuffer )
				{
					s = g_engfuncs.pfnInfoKeyValue( szBuffer, "minplayers" );
					if( s && s[ 0 ] )
					{
						item->minplayers = atoi( s );
						item->minplayers = max( item->minplayers, 0 );
						item->minplayers = min( item->minplayers, gpGlobals->maxClients );
					}
					s = g_engfuncs.pfnInfoKeyValue( szBuffer, "maxplayers" );
					if( s && s[ 0 ] )
					{
						item->maxplayers = atoi( s );
						item->maxplayers = max( item->maxplayers, 0 );
						item->maxplayers = min( item->maxplayers, gpGlobals->maxClients );
					}

					// Remove keys
					//
					g_engfuncs.pfnInfo_RemoveKey( szBuffer, "minplayers" );
					g_engfuncs.pfnInfo_RemoveKey( szBuffer, "maxplayers" );

					strcpy( item->rulebuffer, szBuffer );
				}

				item->next = items;
				items = item;
			}
			else
			{
				ALERT( at_console, "Skipping %s from mapcycle, not a valid map\n", szMap );
			}

		}

		FREE_FILE( aFileList );
	}

	// Fixup circular list pointer
	item = items;

	// Reverse it to get original order
	while( item )
	{
		next = item->next;
		item->next = newlist;
		newlist = item;
		item = next;
	}
	items = newlist;
	item = items;

	// Didn't parse anything
	if( !item )
	{
		return false;
	}

	while( item->next )
	{
		item = item->next;
	}
	item->next = items;

	next_item = item->next;

	return true;
}

void CMapCycle::Clear()
{
	Item_t *p, *n, *start;
	p = items;
	if( p )
	{
		start = p;
		p = p->next;
		while( p != start )
		{
			n = p->next;
			delete p;
			p = n;
		}

		delete items;
	}
	items = nullptr;
	next_item = nullptr;
}

void ExtractCommandString( const char* pszToken, char* pszCommand )
{
	// Now make rules happen
	char	pkey[ 512 ];
	char	value[ 512 ];	// use two buffers so compares
							// work without stomping on each other
	char	*o;

	if( *pszToken == '\\' )
		++pszToken;

	while( 1 )
	{
		o = pkey;
		while( *pszToken != '\\' )
		{
			if( !*pszToken )
				return;
			*o++ = *pszToken++;
		}
		*o = '\0';
		++pszToken;

		o = value;

		while( *pszToken != '\\' && *pszToken )
		{
			if( !*pszToken )
				return;
			*o++ = *pszToken++;
		}
		*o = '\0';

		strcat( pszCommand, pkey );
		if( strlen( value ) > 0 )
		{
			strcat( pszCommand, " " );
			strcat( pszCommand, value );
		}
		strcat( pszCommand, "\n" );

		if( !*pszToken )
			return;
		++pszToken;
	}
}