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
#include <climits>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"
#include "CBasePlayer.h"

LINK_ENTITY_TO_CLASS( player, CBasePlayer );

void CBasePlayer::SetWeaponAnimType( const char* const pszExtension )
{
	ASSERT( pszExtension );

	strncpy( m_szAnimExtension, pszExtension, sizeof( m_szAnimExtension ) - 1 );
	m_szAnimExtension[ sizeof( m_szAnimExtension ) - 1 ] = '\0';
}

CBasePlayerWeapon* CBasePlayer::GetWeapon( int iBucket, int iPosition )
{
	ASSERT( iBucket >= 0 && iBucket < MAX_WEAPON_SLOTS );

	CBasePlayerWeapon* pWeapon = m_rgpPlayerItems[ iBucket ];

	while( pWeapon )
	{
		if( pWeapon->GetWeaponInfo()->GetPosition() == iPosition )
			return pWeapon;

		pWeapon = pWeapon->m_pNext;
	}

	return nullptr;
}

int CBasePlayer::CountAmmo( const int iID ) const
{
	if( iID < 0 )
		return 0;

	ASSERT( g_AmmoTypes.GetAmmoTypeByID( iID ) );

#ifdef CLIENT_DLL
	return m_rgAmmoLast[ iID ];
#else
	return m_rgAmmo[ iID ];
#endif
}

bool CBasePlayer::HasAmmo( CBasePlayerWeapon* pWeapon ) const
{
	if( !pWeapon || !pWeapon->GetWeaponInfo() )
		return false;

	auto pInfo = pWeapon->GetWeaponInfo();

	// weapons with no max ammo can always be selected
	if( !pInfo->GetPrimaryAmmo() || pInfo->GetPrimaryAmmo()->GetMaxCarry() == WEAPON_NOCLIP )
		return true;

#ifdef CLIENT_DLL
	if( pWeapon->m_iClientClip > 0 )
#else
	if( pWeapon->m_iClip > 0 )
#endif
		return true;

	return CountAmmo( pInfo->GetPrimaryAmmo()->GetID() ) ||
		( pInfo->GetSecondaryAmmo() && CountAmmo( pInfo->GetSecondaryAmmo()->GetID() ) ) ||
		( pInfo->GetFlags() & ITEM_FLAG_SELECTONEMPTY );
}

CBasePlayerWeapon* CBasePlayer::GetFirstPos( int iBucket )
{
	CBasePlayerWeapon* pWeapon = m_rgpPlayerItems[ iBucket ];

	CBasePlayerWeapon* pret = nullptr;

	int iLastPos = INT_MAX;

	while( pWeapon )
	{
		if( HasAmmo( pWeapon ) && pWeapon->GetWeaponInfo()->GetPosition() < iLastPos )
		{
			iLastPos = pWeapon->GetWeaponInfo()->GetPosition();
			pret = pWeapon;
		}

		pWeapon = pWeapon->m_pNext;
	}

	return pret;
}

CBasePlayerWeapon* CBasePlayer::GetNextActivePos( int iBucket, int iPosition )
{
	if( iPosition >= MAX_WEAPONS || iBucket >= MAX_WEAPON_SLOTS )
		return nullptr;

	CBasePlayerWeapon* pWeapon = m_rgpPlayerItems[ iBucket ];

	CBasePlayerWeapon* pret = nullptr;

	int iLastPos = INT_MAX;

	while( pWeapon )
	{
		const int iPos = pWeapon->GetWeaponInfo()->GetPosition();

		if( HasAmmo( pWeapon ) && iPos > iPosition && iPos < iLastPos )
		{
			iLastPos = pWeapon->GetWeaponInfo()->GetPosition();
			pret = pWeapon;
		}

		pWeapon = pWeapon->m_pNext;
	}

	return pret;
}

//TODO: remove this. - Solokiller
int CBasePlayer::GetAmmoIndex( const char *psz )
{
	return g_AmmoTypes.GetAmmoID( psz );
}

int CBasePlayer::GetAmmoCount( const char* const pszName ) const
{
	return GetAmmoCountByID( g_AmmoTypes.GetAmmoID( pszName ) );
}

int CBasePlayer::GetAmmoCountByID( const AmmoID_t ammoID ) const
{
	if( ammoID == CAmmoTypes::INVALID_AMMO_ID )
	{
		return -1;
	}

	return m_rgAmmo[ ammoID ];
}

void CBasePlayer::SetAmmoCount( const char* const pszName, const int iCount )
{
	SetAmmoCountByID( g_AmmoTypes.GetAmmoID( pszName ), iCount );
}

void CBasePlayer::SetAmmoCountByID( const AmmoID_t ammoID, const int iCount )
{
	ASSERT( ammoID >= g_AmmoTypes.GetFirstAmmoID() && ammoID <= g_AmmoTypes.GetLastAmmoID() );

	if( ammoID == CAmmoTypes::INVALID_AMMO_ID )
		return;

	m_rgAmmo[ ammoID ] = max( 0, iCount );
}