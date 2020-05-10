#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "CAmmoTypes.h"

CAmmoTypes g_AmmoTypes;

CAmmoType::CAmmoType( const char* const pszName, const AmmoID_t ID, const int iMaxCarry )
	: m_pszName( pszName )
	, m_ID( ID )
	, m_iMaxCarry( iMaxCarry )
{
	ASSERT( pszName );
}

CAmmoTypes::CAmmoTypes()
{
}

CAmmoTypes::~CAmmoTypes()
{
	Clear();
}

bool CAmmoTypes::IsEmpty() const
{
	return m_AmmoList.empty();
}

size_t CAmmoTypes::GetAmmoTypesCount() const
{
	return m_AmmoList.size();
}

AmmoID_t CAmmoTypes::GetFirstAmmoID() const
{
	return !m_AmmoList.empty() ? m_AmmoList.front()->GetID() : INVALID_AMMO_ID;
}

AmmoID_t CAmmoTypes::GetLastAmmoID() const
{
	return !m_AmmoList.empty() ? m_AmmoList.back()->GetID() : INVALID_AMMO_ID;
}

const CAmmoType* CAmmoTypes::GetAmmoTypeByIndex( const size_t uiIndex ) const
{
	if( uiIndex >= m_AmmoList.size() )
		return nullptr;

	auto pType = m_AmmoList[ uiIndex ];

	ASSERT( pType );

	return pType;
}

CAmmoType* CAmmoTypes::GetAmmoTypeByIndex( const size_t uiIndex )
{
	return const_cast<CAmmoType*>( const_cast<const CAmmoTypes*>( this )->GetAmmoTypeByIndex( uiIndex ) );
}

const CAmmoType* CAmmoTypes::GetAmmoTypeByName( const char* const pszName ) const
{
	if( !pszName || !( *pszName ) )
		return nullptr;

	auto it = m_AmmoMap.find( pszName );

	if( it == m_AmmoMap.end() )
		return nullptr;

	ASSERT( it->second < m_AmmoList.size() );

	return m_AmmoList[ it->second ];
}

CAmmoType* CAmmoTypes::GetAmmoTypeByName( const char* const pszName )
{
	return const_cast<CAmmoType*>( const_cast<const CAmmoTypes*>( this )->GetAmmoTypeByName( pszName ) );
}

const CAmmoType* CAmmoTypes::GetAmmoTypeByID( const AmmoID_t ID ) const
{
	if( ID == INVALID_AMMO_ID )
		return nullptr;

	//The ammo ID is its index + 1.
	return GetAmmoTypeByIndex( ID - 1 );
}

CAmmoType* CAmmoTypes::GetAmmoTypeByID( const AmmoID_t ID )
{
	return const_cast<CAmmoType*>( const_cast<const CAmmoTypes*>( this )->GetAmmoTypeByID( ID ) );
}

int CAmmoTypes::GetMaxCarryByName( const char* const pszName ) const
{
	auto pType = GetAmmoTypeByName( pszName );

	if( pType )
		return pType->GetMaxCarry();

	ALERT( at_console, "CAmmoTypes::GetMaxCarryByName() doesn't recognize '%s'!\n", pszName );

	return -1;
}

AmmoID_t CAmmoTypes::GetAmmoID( const char* const pszName ) const
{
	ASSERT( pszName );

	if( !pszName )
		return INVALID_AMMO_ID;

	auto pAmmo = GetAmmoTypeByName( pszName );

	if( !pAmmo )
		return INVALID_AMMO_ID;

	return pAmmo->GetID();
}

CAmmoType* CAmmoTypes::AddAmmoType( const char* const pszName, const int iMaxCarry )
{
	if( !m_bCanAddAmmoTypes )
	{
		//pszName could be null or empty, but we don't care about that.
		ALERT( at_error, "CAmmoTypes::AddAmmoType: Ammo types cannot be added at this time (adding \"%s\")!\n", pszName );
		return nullptr;
	}

	ASSERT( pszName );

	if( !pszName || !( *pszName ) )
		return nullptr;

	auto it = m_AmmoMap.find( pszName );

	if( it != m_AmmoMap.end() )
	{
		return GetAmmoTypeByIndex( it->second );
	}

	//Out of IDs.
	if( m_NextID > LAST_VALID_ID )
	{
		ALERT( at_warning, "CAmmoTypes::AddAmmoType: Out of ammo IDs (%u total) while adding \"%s\"!\n", LAST_VALID_ID - FIRST_VALID_ID, pszName );
		return nullptr;
	}

	auto pType = new CAmmoType( pszName, m_NextID, iMaxCarry );

	m_AmmoList.push_back( pType );

	auto result = m_AmmoMap.insert( std::make_pair( pType->GetName(), m_AmmoList.size() - 1 ) );

	if( result.second )
	{
		//Type successfully added.

		++m_NextID;

		return pType;
	}

	//A problem occurred during insertion.
	ALERT( at_error, "CAmmoTypes::AddAmmoType: An error occurred while adding the ammo type \"%s\" to the list\n", pszName );

	m_AmmoList.erase( m_AmmoList.end() - 1 );

	delete pType;

	return nullptr;
}

void CAmmoTypes::Clear()
{
	m_AmmoMap.clear();

	for( auto pType : m_AmmoList )
	{
		delete pType;
	}

	m_AmmoList.clear();

	m_NextID = FIRST_VALID_ID;
}

size_t CAmmoTypes::GenerateHash() const
{
	size_t uiHash = 0;

	//TODO: not the best hash, but it'll do. - Solokiller
	for( const auto& ammo : m_AmmoList )
	{
		uiHash += StringHash( ammo->GetName() );
	}

	return uiHash;
}