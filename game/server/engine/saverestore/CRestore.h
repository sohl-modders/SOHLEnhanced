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
#ifndef GAME_SERVER_ENGINE_SAVERESTORE_CRESTORE_H
#define GAME_SERVER_ENGINE_SAVERESTORE_CRESTORE_H

#include "CSaveRestoreBuffer.h"

namespace engine
{
struct HEADER
{
	unsigned short		size;
	unsigned short		token;
	char				*pData;
};

/**
*	Buffer used for restoring operations.
*	Used for engine save/restore operations.
*/
class CRestore : public CSaveRestoreBuffer
{
public:
	CRestore( SAVERESTOREDATA *pdata ) : CSaveRestoreBuffer( pdata ) { m_global = 0; m_precache = true; }
	bool	ReadEntVars( const char *pname, entvars_t *pev );		// entvars_t
	bool	ReadFields( const char *pname, void *pBaseData, const TYPEDESCRIPTION *pFields, int fieldCount );
	int		ReadField( void *pBaseData, const TYPEDESCRIPTION *pFields, int fieldCount, int startField, int size, char *pName, void *pData );
	int		ReadInt( void );
	short	ReadShort( void );
	int		ReadNamedInt( const char *pName );
	char	*ReadNamedString( const char *pName );
	int		Empty( void ) { return ( m_pdata == nullptr ) || ( ( m_pdata->pCurrentData - m_pdata->pBaseData ) >= m_pdata->bufferSize ); }
	inline	void SetGlobalMode( int global ) { m_global = global; }
	void	PrecacheMode( const bool mode ) { m_precache = mode; }

private:
	char	*BufferPointer( void );
	void	BufferReadBytes( char *pOutput, int size );
	void	BufferSkipBytes( int bytes );
	int		BufferSkipZString( void );
	int		BufferCheckZString( const char *string );

	void	BufferReadHeader( HEADER *pheader );

	int		m_global;		// Restoring a global entity?
	bool	m_precache;
};
}

#endif //GAME_SERVER_ENGINE_SAVERESTORE_CRESTORE_H
