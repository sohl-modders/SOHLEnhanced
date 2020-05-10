#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CEntityRegistry.h"

#include "CEntityDictionary.h"



CEntityDictionary& GetEntityDict()
{
	static CEntityDictionary g_EntityDict;

	return g_EntityDict;
}

CBaseEntityRegistry* CEntityDictionary::FindEntityClassByEntityName( const char* const pszEntityName )
{
	ASSERT( pszEntityName );

	if( !pszEntityName )
		return nullptr;

	auto it = m_Entities.find( pszEntityName );

	if( it != m_Entities.end() )
		return it->second;

	return nullptr;
}

bool CEntityDictionary::AddEntityClass( CBaseEntityRegistry* pEntity )
{
	ASSERT( pEntity );

	if( FindEntityClassByEntityName( pEntity->GetEntityname() ) )
	{
		Alert( at_error, "CEntityDictionary::AddEntityClass: Attempted to add duplicate entity class \"%s\" (C++ class: \"%s\")!\n", pEntity->GetEntityname(), pEntity->GetClassname() );
		return false;
	}

	auto result = m_Entities.insert( std::make_pair( pEntity->GetEntityname(), pEntity ) );

	if( !result.second )
	{
		Alert( at_error, "CEntityDictionary::AddEntityClass: Failed to insert entity class \"%s\" (C++ class: \"%s\")!\n", pEntity->GetEntityname(), pEntity->GetClassname() );
	}

	return result.second;
}

CBaseEntity* CEntityDictionary::CreateInstance( const char* const pszEntityName, entvars_t* pev )
{
	if( auto pClass = FindEntityClassByEntityName( pszEntityName ) )
		return pClass->CreateInstance( pev );

	return nullptr;
}

CBaseEntity* CEntityDictionary::CreateInstance( const char* const pszEntityName, edict_t* pEdict )
{
	if( auto pClass = FindEntityClassByEntityName( pszEntityName ) )
		return pClass->CreateInstance( pEdict );

	return nullptr;
}

CBaseEntity* CEntityDictionary::CreateInstance( const char* const pszEntityName )
{
	if( auto pClass = FindEntityClassByEntityName( pszEntityName ) )
		return pClass->CreateInstance();

	return nullptr;
}

void CEntityDictionary::EnumEntityClasses( EntityEnumCallback pCallback )
{
	ASSERT( pCallback );

	if( !pCallback )
		return;

	for( auto& reg : m_Entities )
	{
		if( !pCallback( *reg.second ) )
			break;
	}
}