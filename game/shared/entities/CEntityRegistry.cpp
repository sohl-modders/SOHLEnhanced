#include <cassert>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CEntityDictionary.h"

#include "CEntityRegistry.h"

CBaseEntityRegistry::CBaseEntityRegistry( const char* const pszEntityName, const char* const pszClassName )
	: m_pszEntityName( pszEntityName )
	, m_pszClassName( pszClassName )
{
	assert( pszEntityName );
	assert( *pszEntityName );
	assert( pszClassName );
	assert( *pszClassName );

	GetEntityDict().AddEntityClass( this );
}