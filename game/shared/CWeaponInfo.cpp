#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "entities/weapons/CAmmoTypes.h"

#ifdef CLIENT_DLL
#include "hud.h"
#include "CWeaponHUDInfo.h"
#endif

#include "CWeaponInfo.h"

CWeaponInfo::~CWeaponInfo()
{
#ifdef CLIENT_DLL
	delete m_pHUDInfo;
#endif
}

bool CWeaponInfo::KeyValue( const char* const pszKey, const char* const pszValue )
{
	if( FStrEq( pszKey, "bucket" ) )
	{
		m_iBucket = atoi( pszValue );
		return true;
	}
	else if( FStrEq( pszKey, "position" ) )
	{
		m_iPosition = atoi( pszValue );
		return true;
	}
	else if( FStrEq( pszKey, "primary_ammo" ) )
	{
		m_pPrimaryAmmo = g_AmmoTypes.GetAmmoTypeByName( pszValue );

		if( !m_pPrimaryAmmo )
		{
			Alert( at_error, "CWeaponInfo::KeyValue: Primary ammo \"%s\" does not exist!\n", pszValue );
		}

		return true;
	}
	else if( FStrEq( pszKey, "secondary_ammo" ) )
	{
		m_pSecondaryAmmo = g_AmmoTypes.GetAmmoTypeByName( pszValue );

		if( !m_pSecondaryAmmo )
		{
			Alert( at_error, "CWeaponInfo::KeyValue: Secondary ammo \"%s\" does not exist!\n", pszValue );
		}

		return true;
	}
	else if( FStrEq( pszKey, "max_magazine" ) )
	{
		m_iMaxMagazine = atoi( pszValue );

		if( m_iMaxMagazine < WEAPON_NOCLIP )
		{
			Alert( at_warning, "CWeaponInfo::KeyValue: Max magazine value \"%d\" is invalid!\n", m_iMaxMagazine );
			m_iMaxMagazine = WEAPON_NOCLIP;
		}

		return true;
	}
	else if( FStrEq( pszKey, "default_ammo" ) )
	{
		m_iDefaultAmmo = atoi( pszValue );

		if( m_iDefaultAmmo < 0 )
		{
			Alert( at_warning, "CWeaponInfo::KeyValue: Default ammo value \"%d\" is invalid!\n", m_iDefaultAmmo );
			m_iDefaultAmmo = 0;
		}

		return true;
	}
	else if( FStrEq( pszKey, "weight" ) )
	{
		m_iWeight = atoi( pszValue );
		return true;
	}
	else if( FStrEq( pszKey, "flag" ) )
	{
		return Flag( pszValue );
	}

	return false;
}

bool CWeaponInfo::Flag( const char* const pszFlag )
{
	const ItemInfoFlag flag = StringToItemInfoFlag( pszFlag );

	if( flag != ITEM_FLAG_NONE )
	{
		m_iFlags |= flag;
		return true;
	}

	return false;
}

void CWeaponInfo::SetWeaponName( const char* const pszWeaponName )
{
	ASSERT( pszWeaponName );

	m_pszWeaponName = g_StringPool.Allocate( pszWeaponName );
}