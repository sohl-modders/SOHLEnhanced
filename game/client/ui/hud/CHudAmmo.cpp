/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
//
// Ammo.cpp
//
// implementation of CHudAmmo class
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "pm_shared.h"

#include <climits>
#include <string.h>
#include <stdio.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"
#include "CBasePlayer.h"

#include "CWeaponHUDInfo.h"

#include "hl/CClientPrediction.h"

#include "ammohistory.h"
#include "vgui_TeamFortressViewport.h"

#include "CWeaponInfoCache.h"

#include "renderer/SpriteRenderUtils.h"

#include "CHudMenu.h"
#include "CHudAmmo.h"

//TODO: shouldn't be global - Solokiller

int g_weaponselect = 0;

DECLARE_COMMAND(CHudAmmo, Slot1);
DECLARE_COMMAND(CHudAmmo, Slot2);
DECLARE_COMMAND(CHudAmmo, Slot3);
DECLARE_COMMAND(CHudAmmo, Slot4);
DECLARE_COMMAND(CHudAmmo, Slot5);
DECLARE_COMMAND(CHudAmmo, Slot6);
DECLARE_COMMAND(CHudAmmo, Slot7);
DECLARE_COMMAND(CHudAmmo, Slot8);
DECLARE_COMMAND(CHudAmmo, Slot9);
DECLARE_COMMAND(CHudAmmo, Slot10);
DECLARE_COMMAND(CHudAmmo, Close);
DECLARE_COMMAND(CHudAmmo, NextWeapon);
DECLARE_COMMAND(CHudAmmo, PrevWeapon);

// width of ammo fonts
#define AMMO_SMALL_WIDTH 10
#define AMMO_LARGE_WIDTH 20

CHudAmmo::CHudAmmo( const char* const pszName, CHLHud& hud )
	: BaseClass( pszName, hud )
{
}

void CHudAmmo::Init()
{
	HOOK_MESSAGE( CurWeapon );		// Current weapon and clip
	HOOK_MESSAGE( AmmoPickup );		// flashes an ammo pickup record
	HOOK_MESSAGE( WeapPickup );		// flashes a weapon pickup record
	HOOK_MESSAGE( ItemPickup );
	HOOK_MESSAGE( HideWeapon );		// hides the weapon, ammo, and crosshair displays temporarily
	HOOK_MESSAGE( AmmoX );			// update known ammo type's count

	HOOK_COMMAND("slot1", Slot1);
	HOOK_COMMAND("slot2", Slot2);
	HOOK_COMMAND("slot3", Slot3);
	HOOK_COMMAND("slot4", Slot4);
	HOOK_COMMAND("slot5", Slot5);
	HOOK_COMMAND("slot6", Slot6);
	HOOK_COMMAND("slot7", Slot7);
	HOOK_COMMAND("slot8", Slot8);
	HOOK_COMMAND("slot9", Slot9);
	HOOK_COMMAND("slot10", Slot10);
	HOOK_COMMAND("cancelselect", Close);
	HOOK_COMMAND("invnext", NextWeapon);
	HOOK_COMMAND("invprev", PrevWeapon);

	Reset();

	CVAR_CREATE( "hud_fastswitch", "0", FCVAR_ARCHIVE );		// controls whether or not weapons can be selected in one keypress

	m_pCrosshair = gEngfuncs.pfnGetCvarPointer( "crosshair" );
	m_pCrosshairMode = CVAR_CREATE( "crosshair_mode", "0", FCVAR_ARCHIVE );
	m_pCrosshairScale = CVAR_CREATE( "crosshair_scale", "1", FCVAR_ARCHIVE );

	GetFlags() |= HUD_ACTIVE; //!!!

	if( CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer() )
	{
		pPlayer->GetWeapons().ClearAll();
	}

	gHR.Init();
}

void CHudAmmo::Reset()
{
	m_fFade = 0;
	GetFlags() |= HUD_ACTIVE; //!!!

	m_pActiveSel = nullptr;
	GetHud().GetHideHudBits() = 0;

	if( CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer() )
	{
		pPlayer->GetWeapons().ClearAll();
	}

	m_pWeapon = nullptr;

	gHR.Reset();
}

void CHudAmmo::VidInit()
{
	// Load sprites for buckets (top row of weapon menu)
	m_HUD_bucket0 = GetHud().GetSpriteIndex( "bucket1" );
	m_HUD_selection = GetHud().GetSpriteIndex( "selection" );

	//TODO: get the sprite once instead of fetching it over and over - Solokiller
	m_hsprBuckets = GetHud().GetSprite(m_HUD_bucket0);
	m_iBucketWidth = GetHud().GetSpriteRect(m_HUD_bucket0).right - GetHud().GetSpriteRect(m_HUD_bucket0).left;
	m_iBucketHeight = GetHud().GetSpriteRect(m_HUD_bucket0).bottom - GetHud().GetSpriteRect(m_HUD_bucket0).top;

	gHR.iHistoryGap = max( gHR.iHistoryGap, GetHud().GetSpriteRect(m_HUD_bucket0).bottom - GetHud().GetSpriteRect(m_HUD_bucket0).top);

	if (ScreenWidth >= 640)
	{
		m_iABWidth = 20;
		m_iABHeight = 4;
	}
	else
	{
		m_iABWidth = 10;
		m_iABHeight = 2;
	}

	m_pWeapon = nullptr;
}

//
// Think:
//  Used for selection of weapon menu item.
//
void CHudAmmo::Think()
{
	if( m_bNeedsLocalUpdate )
	{
		m_bNeedsLocalUpdate = false;

		if( m_pWeapon )
			UpdateWeaponHUD( m_pWeapon, m_bOnTarget );
	}

	if ( m_bPlayerDead )
		return;

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	if ( Hud().GetWeaponBits() != pPlayer->GetWeapons().Get() )
	{
		pPlayer->GetWeapons().Set( Hud().GetWeaponBits() );

		for (int i = MAX_WEAPONS-1; i > 0; i-- )
		{
			CBasePlayerWeapon *p = g_Prediction.GetWeapon(i);

			if ( p && p->GetWeaponInfo() )
			{
				if ( Hud().GetWeaponBits() & ( 1 << p->GetWeaponInfo()->GetID() ) )
					pPlayer->AddPlayerItem( p );
				else
					pPlayer->RemovePlayerItem( p );
			}
		}
	}

	if( !m_pActiveSel )
		return;

	// has the player selected one?
	if (Hud().GetKeyBits() & IN_ATTACK)
	{
		if ( m_pActiveSel != (CBasePlayerWeapon *)1)
		{
			ServerCmd( m_pActiveSel->GetWeaponInfo()->GetWeaponName());
			g_weaponselect = m_pActiveSel->GetWeaponInfo()->GetID();
		}

		m_pLastSel = m_pActiveSel;
		m_pActiveSel = nullptr;
		Hud().ClearKeyBits( IN_ATTACK );

		PlaySound("common/wpn_select.wav", 1);
	}

}

// Menu Selection Code

void CHudAmmo::SelectSlot( int iSlot, const bool fAdvance, int iDirection )
{
	if( auto pMenu = GETHUDCLASS( CHudMenu ) )
	{
		if ( pMenu->m_fMenuDisplayed && !fAdvance && (iDirection == 1) )
		{ // menu is overriding slot use commands
			pMenu->SelectMenuItem( iSlot + 1 );  // slots are one off the key numbers
			return;
		}
	}

	if ( iSlot > MAX_WEAPON_SLOTS )
		return;

	if ( m_bPlayerDead || GetHud().GetHideHudBits().Any( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
		return;

	if (!( Hud().GetWeaponBits() & (1<<(WEAPON_SUIT)) ))
		return;

	if ( ! ( Hud().GetWeaponBits() & ~(1<<(WEAPON_SUIT)) ))
		return;

	CBasePlayerWeapon *p = NULL;
	bool fastSwitch = CVAR_GET_FLOAT( "hud_fastswitch" ) != 0;

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	if ( ( m_pActiveSel == NULL) || ( m_pActiveSel == ( CBasePlayerWeapon *)1) || (iSlot != m_pActiveSel->GetWeaponInfo()->GetBucket()) )
	{
		PlaySound( "common/wpn_hudon.wav", 1 );
		p = pPlayer->GetFirstPos( iSlot );

		if ( p && fastSwitch ) // check for fast weapon switch mode
		{
			// if fast weapon switch is on, then weapons can be selected in a single keypress
			// but only if there is only one item in the bucket
			CBasePlayerWeapon *p2 = pPlayer->GetNextActivePos( p->GetWeaponInfo()->GetBucket(), p->GetWeaponInfo()->GetPosition() );
			if ( !p2 )
			{	// only one active item in bucket, so change directly to weapon
				ServerCmd( p->GetWeaponInfo()->GetWeaponName() );
				g_weaponselect = p->GetWeaponInfo()->GetID();
				return;
			}
		}
	}
	else
	{
		PlaySound("common/wpn_moveselect.wav", 1);
		if ( m_pActiveSel )
			p = pPlayer->GetNextActivePos( m_pActiveSel->GetWeaponInfo()->GetBucket(), m_pActiveSel->GetWeaponInfo()->GetPosition() );
		if ( !p )
			p = pPlayer->GetFirstPos( iSlot );
	}

	
	if ( !p )  // no selection found
	{
		// just display the weapon list, unless fastswitch is on just ignore it
		if ( !fastSwitch )
			m_pActiveSel = ( CBasePlayerWeapon *)1;
		else
			m_pActiveSel = nullptr;
	}
	else 
		m_pActiveSel = p;
}

//------------------------------------------------------------------------
// Message Handlers
//------------------------------------------------------------------------

//
// AmmoX  -- Update the count of a known type of ammo
// 
void CHudAmmo::MsgFunc_AmmoX(const char *pszName, int iSize, void *pbuf)
{
	CBufferReader reader( pbuf, iSize );

	int iIndex = reader.ReadByte();
	int iCount = reader.ReadByte();

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	pPlayer->m_rgAmmoLast[ iIndex ] = abs(iCount);
}

void CHudAmmo::MsgFunc_AmmoPickup( const char *pszName, int iSize, void *pbuf )
{
	CBufferReader reader( pbuf, iSize );
	int iIndex = reader.ReadByte();
	int iCount = reader.ReadByte();

	// Add ammo to the history
	gHR.AddToHistory( HISTSLOT_AMMO, iIndex, abs(iCount) );
}

void CHudAmmo::MsgFunc_WeapPickup( const char *pszName, int iSize, void *pbuf )
{
	CBufferReader reader( pbuf, iSize );
	int iIndex = reader.ReadByte();

	// Add the weapon to the history
	gHR.AddToHistory( HISTSLOT_WEAP, iIndex );
}

void CHudAmmo::MsgFunc_ItemPickup( const char *pszName, int iSize, void *pbuf )
{
	CBufferReader reader( pbuf, iSize );
	const char *szName = reader.ReadString();

	// Add the weapon to the history
	gHR.AddToHistory( HISTSLOT_ITEM, szName );
}


void CHudAmmo::MsgFunc_HideWeapon( const char *pszName, int iSize, void *pbuf )
{
	CBufferReader reader( pbuf, iSize );
	
	GetHud().GetHideHudBits() = reader.ReadByte();

	if (gEngfuncs.IsSpectateOnly())
		return;

	if ( GetHud().GetHideHudBits().Any( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
	{
		wrect_t nullrc = {};
		m_pActiveSel = nullptr;
		SetCrosshair( 0, nullrc, 0, 0, 0 );
	}
	else
	{
		if ( m_pWeapon )
		{
			const auto& crosshair = m_pWeapon->GetWeaponInfo()->GetHUDInfo()->GetCrosshair();
			SetCrosshair( crosshair.hSprite, crosshair.rect, 255, 255, 255 );
		}
	}
}

// 
//  CurWeapon: Update hud state with the current weapon and clip count. Ammo
//  counts are updated with AmmoX. Server assures that the Weapon ammo type 
//  numbers match a real ammo type.
//
bool CHudAmmo::MsgFunc_CurWeapon(const char *pszName, int iSize, void *pbuf )
{
	wrect_t nullrc = {};

	CBufferReader reader( pbuf, iSize );

	const WpnOnTargetState state = static_cast<WpnOnTargetState>( reader.ReadByte() );
	int iId = reader.ReadChar();
	int iClip = reader.ReadChar();

	// detect if we're also on target
	bool fOnTarget = state == WpnOnTargetState::ACTIVE_IS_ONTARGET;

	if ( iId < 1 )
	{
		SetCrosshair(0, nullrc, 0, 0, 0);
		//Clear out the weapon so we don't keep drawing the last active weapon's ammo. - Solokiller
		m_pWeapon = nullptr;
		return false;
	}

	if ( g_iUser1 != OBS_IN_EYE )
	{
		// Is player dead???
		if ((iId == -1) && (iClip == -1))
		{
			m_bPlayerDead = true;
			m_pActiveSel = nullptr;
			return true;
		}
		m_bPlayerDead = false;
	}

	CBasePlayerWeapon *pWeapon = g_Prediction.GetWeapon( iId );

	if( !pWeapon )
	{
		m_bNeedsLocalUpdate = true;
		m_bOnTarget = fOnTarget;
		GetFlags() |= HUD_ACTIVE;
		return true;
	}
	else if( m_bNeedsLocalUpdate )
		m_bNeedsLocalUpdate = false;

	if ( iClip < -1 )
		pWeapon->m_iClientClip = abs(iClip);
	else
		pWeapon->m_iClientClip = iClip;


	if ( state == WpnOnTargetState::NOT_ACTIVE_WEAPON )	// we're not the current weapon, so update no more
		return true;

	m_pWeapon = pWeapon;

	UpdateWeaponHUD( m_pWeapon, fOnTarget );
	
	return true;
}

void CHudAmmo::UpdateWeaponHUD( CBasePlayerWeapon* pWeapon, bool bOnTarget )
{
	auto pHUDInfo = pWeapon->GetWeaponInfo()->GetHUDInfo();

	//TODO: define 90 constant - Solokiller
	if( Hud().GetFOV() >= 90 )
	{ // normal crosshairs
		if( bOnTarget && pHUDInfo->GetAutoAim().hSprite )
			SetCrosshair( pHUDInfo->GetAutoAim().hSprite, pHUDInfo->GetAutoAim().rect, 255, 255, 255 );
		else
			SetCrosshair( pHUDInfo->GetCrosshair().hSprite, pHUDInfo->GetCrosshair().rect, 255, 255, 255 );
	}
	else
	{ // zoomed crosshairs
		if( bOnTarget && pHUDInfo->GetZoomedAutoAim().hSprite )
			SetCrosshair( pHUDInfo->GetZoomedAutoAim().hSprite, pHUDInfo->GetZoomedAutoAim().rect, 255, 255, 255 );
		else
			SetCrosshair( pHUDInfo->GetZoomedCrosshair().hSprite, pHUDInfo->GetZoomedCrosshair().rect, 255, 255, 255 );

	}

	m_fFade = 200.0f; //!!!
	GetFlags() |= HUD_ACTIVE;
}

//------------------------------------------------------------------------
// Command Handlers
//------------------------------------------------------------------------
// Slot button pressed
void CHudAmmo::SlotInput( int iSlot )
{
	if ( gViewPort && gViewPort->SlotInput( iSlot ) )
		return;

	SelectSlot( iSlot, false, 1 );
}

void CHudAmmo::UserCmd_Slot1(void)
{
	SlotInput( 0 );
}

void CHudAmmo::UserCmd_Slot2(void)
{
	SlotInput( 1 );
}

void CHudAmmo::UserCmd_Slot3(void)
{
	SlotInput( 2 );
}

void CHudAmmo::UserCmd_Slot4(void)
{
	SlotInput( 3 );
}

void CHudAmmo::UserCmd_Slot5(void)
{
	SlotInput( 4 );
}

void CHudAmmo::UserCmd_Slot6(void)
{
	SlotInput( 5 );
}

void CHudAmmo::UserCmd_Slot7(void)
{
	SlotInput( 6 );
}

void CHudAmmo::UserCmd_Slot8(void)
{
	SlotInput( 7 );
}

void CHudAmmo::UserCmd_Slot9(void)
{
	SlotInput( 8 );
}

void CHudAmmo::UserCmd_Slot10(void)
{
	SlotInput( 9 );
}

void CHudAmmo::UserCmd_Close(void)
{
	if ( m_pActiveSel )
	{
		m_pLastSel = m_pActiveSel;
		m_pActiveSel = nullptr;
		PlaySound("common/wpn_hudoff.wav", 1);
	}
	else
		EngineClientCmd("escape");
}


// Selects the next item in the weapon menu
void CHudAmmo::UserCmd_NextWeapon(void)
{
	if ( m_bPlayerDead || GetHud().GetHideHudBits().Any( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
		return;

	if ( !m_pActiveSel || m_pActiveSel == ( CBasePlayerWeapon*)1 )
		m_pActiveSel = m_pWeapon;

	int slot = 0;

	CBasePlayerWeapon* pWeapon = nullptr;

	if ( m_pActiveSel )
	{
		slot = m_pActiveSel->GetWeaponInfo()->GetBucket();
		pWeapon = m_pActiveSel->m_pNext;

		if( !pWeapon )
		{
			if( slot == ( MAX_WEAPON_SLOTS - 1 ) )
				slot = 0;
			else
				++slot;
		}
	}

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	for ( int loop = 0; loop <= 1; loop++ )
	{
		for ( ; slot < MAX_WEAPON_SLOTS; slot++ )
		{
			if( !pWeapon )
				pWeapon = pPlayer->m_rgpPlayerItems[ slot ];

			for ( ; pWeapon; pWeapon = pWeapon->m_pNext )
			{
				if ( pWeapon && pPlayer->HasAmmo( pWeapon ) )
				{
					m_pActiveSel = pWeapon;
					return;
				}
			}

			pWeapon = nullptr;
		}

		slot = 0;  // start looking from the first slot again
	}

	m_pActiveSel = nullptr;
}

// Selects the previous item in the menu
void CHudAmmo::UserCmd_PrevWeapon(void)
{
	if ( m_bPlayerDead || GetHud().GetHideHudBits().Any( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
		return;

	if ( !m_pActiveSel || m_pActiveSel == ( CBasePlayerWeapon*)1 )
		m_pActiveSel = m_pWeapon;

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	CBasePlayerWeapon* pWeapon = nullptr;
	int slot = MAX_WEAPON_SLOTS-1;

	if ( m_pActiveSel )
	{
		slot = m_pActiveSel->GetWeaponInfo()->GetBucket();
		for( CBasePlayerWeapon* pWpn = pPlayer->m_rgpPlayerItems[ slot ], * pPrev = nullptr; pWpn; pPrev = pWpn, pWpn = pWpn->m_pNext )
		{
			if( pWpn == m_pActiveSel )
			{
				pWeapon = pPrev;
				break;
			}
		}

		if( !pWeapon )
		{
			if( slot == 0 )
				slot = MAX_WEAPON_SLOTS - 1;
			else
				--slot;
		}
	}
	
	for ( int loop = 0; loop <= 1; loop++ )
	{
		for ( ; slot >= 0; slot-- )
		{
			//TODO: consider making the weapon list a double linked list, or turn the weapon list into a single array of pointers. - Solokiller
			if( !pWeapon )
			{
				pWeapon = pPlayer->m_rgpPlayerItems[ slot ];

				while( pWeapon && pWeapon->m_pNext )
					pWeapon = pWeapon->m_pNext;
			}

			while( pWeapon )
			{
				if( pPlayer->HasAmmo( pWeapon ) )
				{
					m_pActiveSel = pWeapon;
					return;
				}

				auto pWpn = pPlayer->m_rgpPlayerItems[ slot ];

				while( pWpn && pWpn->m_pNext && pWpn != pWeapon && pWpn->m_pNext != pWeapon )
					pWpn = pWpn->m_pNext;

				if( pWpn != pWeapon )
					pWeapon = pWpn;
				else
					pWeapon = nullptr;
			}
		}
		
		slot = MAX_WEAPON_SLOTS-1;
	}

	m_pActiveSel = nullptr;
}

//-------------------------------------------------------------------------
// Drawing code
//-------------------------------------------------------------------------

bool CHudAmmo::Draw(float flTime)
{
	int a, x, y, r, g, b;
	int AmmoWidth;

	if (!( Hud().GetWeaponBits() & (1<<(WEAPON_SUIT)) ))
		return true;

	if ( GetHud().GetHideHudBits().Any( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
		return true;

	// Draw Weapon Menu
	DrawWList(flTime);

	// Draw ammo pickup history
	gHR.DrawAmmoHistory( flTime );

	if (!( GetFlags() & HUD_ACTIVE))
		return false;

	if (!m_pWeapon)
		return false;

	if( m_pCrosshair->value != 0 && m_hCrosshair != INVALID_HSPRITE )
	{
		float flScale;

		switch( static_cast<int>( m_pCrosshairMode->value ) )
		{
		default:
		case CROSS_NATIVESCALE:
			flScale = 1;
			break;

		case CROSS_RESSCALE:
			flScale = max( ( ScreenWidth / 640.0 ) * 0.75, 1.0 );
			break;

		case CROSS_USERSCALE:
			flScale = fabs( m_pCrosshairScale->value );
			break;
		}

		class CCrosshairTriCallback : public ITriCoordFallback
		{
		public:
			void Calculate( HSPRITE hSprite, const int frame, const wrect_t& rect, const float flScale, int& x, int& y ) override
			{
				x = ( ScreenWidth - ( ( rect.right - rect.left ) ) ) / 2;
				y = ( ScreenHeight - ( ( rect.bottom - rect.top ) ) ) / 2;
			}
		};

		CCrosshairTriCallback callback;

		Tri_DrawScaledSprite( m_hCrosshair, 0, m_iR, m_iG, m_iB, 255, kRenderTransTexture, flScale, callback, &m_CrosshairRC );
	}

	CBasePlayerWeapon *pw = m_pWeapon; // shorthand

	// SPR_Draw Ammo
	if( !pw->GetWeaponInfo()->GetPrimaryAmmo() && !pw->GetWeaponInfo()->GetSecondaryAmmo() )
		return false;

	int iFlags = DHN_DRAWZERO; // draw 0 values

	AmmoWidth = GetHud().GetSpriteRect( GetHud().GetHudNumber0Index() ).right - GetHud().GetSpriteRect( GetHud().GetHudNumber0Index() ).left;

	a = (int) max( static_cast<float>( MIN_ALPHA ), m_fFade );

	if (m_fFade > 0)
		m_fFade -= ( Hud().GetTimeDelta() * 20 );

	GetHud().GetPrimaryColor().UnpackRGB(r,g,b);

	ScaleColors(r, g, b, a );

	// Does this weapon have a clip?
	y = ScreenHeight - GetHud().GetFontHeight() - GetHud().GetFontHeight() /2;

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	// Does weapon have any ammo at all?
	if ( auto pAmmo = pw->GetWeaponInfo()->GetPrimaryAmmo() )
	{
		const auto& ammo = m_pWeapon->GetWeaponInfo()->GetHUDInfo()->GetPrimaryAmmo();

		int iIconWidth = ammo.rect.right - ammo.rect.left;
		
		if (pw->m_iClientClip >= 0)
		{
			// room for the number and the '|' and the current ammo
			
			x = ScreenWidth - (8 * AmmoWidth) - iIconWidth;
			x = GetHud().DrawHudNumber(x, y, iFlags | DHN_3DIGITS, pw->m_iClientClip, r, g, b);

			int iBarWidth =  AmmoWidth/10;

			x += AmmoWidth/2;

			GetHud().GetPrimaryColor().UnpackRGB(r,g,b);

			// draw the | bar
			FillRGBA(x, y, iBarWidth, GetHud().GetFontHeight(), r, g, b, a);

			x += iBarWidth + AmmoWidth/2;

			// GL Seems to need this
			ScaleColors(r, g, b, a );
			x = GetHud().DrawHudNumber(x, y, iFlags | DHN_3DIGITS, pPlayer->CountAmmo( pAmmo->GetID() ), r, g, b);


		}
		else
		{
			// SPR_Draw a bullets only line
			x = ScreenWidth - 4 * AmmoWidth - iIconWidth;
			x = GetHud().DrawHudNumber(x, y, iFlags | DHN_3DIGITS, pPlayer->CountAmmo( pAmmo->GetID() ), r, g, b);
		}

		// Draw the ammo Icon
		int iOffset = ( ammo.rect.bottom - ammo.rect.top)/8;
		SPR_Set( ammo.hSprite, r, g, b);
		SPR_DrawAdditive(0, x, y - iOffset, &ammo.rect );
	}

	// Does weapon have seconday ammo?
	if ( auto pAmmo = pw->GetWeaponInfo()->GetSecondaryAmmo() )
	{
		const auto& ammo2 = m_pWeapon->GetWeaponInfo()->GetHUDInfo()->GetSecondaryAmmo();

		int iIconWidth = ammo2.rect.right - ammo2.rect.left;

		// Do we have secondary ammo?
		if ( pPlayer->CountAmmo( pAmmo->GetID() ) > 0 )
		{
			y -= GetHud().GetFontHeight() + GetHud().GetFontHeight() /4;
			x = ScreenWidth - 4 * AmmoWidth - iIconWidth;
			x = GetHud().DrawHudNumber(x, y, iFlags|DHN_3DIGITS, pPlayer->CountAmmo( pAmmo->GetID() ), r, g, b);

			// Draw the ammo Icon
			SPR_Set( ammo2.hSprite, r, g, b);
			int iOffset = ( ammo2.rect.bottom - ammo2.rect.top)/8;
			SPR_DrawAdditive(0, x, y - iOffset, &ammo2.rect );
		}
	}
	return true;
}

//
// Draws the ammo bar on the hud
//
int CHudAmmo::DrawBar(int x, int y, int width, int height, float f)
{
	int r, g, b;

	if (f < 0)
		f = 0;
	if (f > 1)
		f = 1;

	if (f)
	{
		int w = f * width;

		// Always show at least one pixel if we have ammo.
		if (w <= 0)
			w = 1;
		const auto& barColor = GetHud().GetAmmoBarColor();

		FillRGBA(x, y, w, height, barColor.r(), barColor.g(), barColor.b(), 255);
		x += w;
		width -= w;
	}

	GetHud().GetPrimaryColor().UnpackRGB(r, g, b);

	FillRGBA(x, y, width, height, r, g, b, 128);

	return (x + width);
}

void CHudAmmo::DrawAmmoBar( CBasePlayerWeapon *p, int x, int y, int width, int height)
{
	if ( !p )
		return;

	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();
	
	if ( auto pAmmo = p->GetWeaponInfo()->GetPrimaryAmmo() )
	{
		if( !pPlayer->CountAmmo( pAmmo->GetID() ) )
			return;

		float f = (float)pPlayer->CountAmmo( pAmmo->GetID() )/(float) pAmmo->GetMaxCarry();
		
		x = DrawBar(x, y, width, height, f);


		// Do we have secondary ammo too?

		if ( auto pAmmo2 = p->GetWeaponInfo()->GetSecondaryAmmo() )
		{
			f = (float)pPlayer->CountAmmo( pAmmo2->GetID() )/(float) pAmmo2->GetMaxCarry();

			x += 5; //!!!

			DrawBar(x, y, width, height, f);
		}
	}
}

//
// Draw Weapon Menu
//
int CHudAmmo::DrawWList(float flTime)
{
	int r,g,b,x,y,a,i;

	if ( !m_pActiveSel )
		return 0;

	int iActiveSlot;

	if ( m_pActiveSel == ( CBasePlayerWeapon *)1 )
		iActiveSlot = -1;	// current slot has no weapons
	else 
		iActiveSlot = m_pActiveSel->GetWeaponInfo()->GetBucket();

	x = 10; //!!!
	y = 10; //!!!
	
	CBasePlayer* pPlayer = g_Prediction.GetLocalPlayer();

	// Ensure that there are available choices in the active slot
	if ( iActiveSlot > 0 )
	{
		if ( !pPlayer->GetFirstPos( iActiveSlot ) )
		{
			m_pActiveSel = ( CBasePlayerWeapon *)1;
			iActiveSlot = -1;
		}
	}

	// Shepard : determine how many buckets we should draw
	int iBucketsToDraw = 0;
	for ( i = 0; i < MAX_WEAPON_SLOTS; i++ )
	{
		for ( int j = 0; j < MAX_WEAPONS; j++ )
		{
			CBasePlayerWeapon *pWeapon = pPlayer->GetWeapon( i, j );
			if ( pWeapon && pWeapon->GetWeaponInfo() && pWeapon->GetWeaponInfo()->GetBucket() > iBucketsToDraw )
				iBucketsToDraw = pWeapon->GetWeaponInfo()->GetBucket();
		}
	}
	iBucketsToDraw++;

	// Draw top line
	for ( i = 0; i < iBucketsToDraw; i++ )
	{
		int iWidth;

		GetHud().GetPrimaryColor().UnpackRGB(r,g,b);
	
		if ( iActiveSlot == i )
			a = 255;
		else
			a = 192;

		ScaleColors(r, g, b, 255);
		SPR_Set( GetHud().GetSprite(m_HUD_bucket0 + i), r, g, b );

		// make active slot wide enough to accomodate gun pictures
		if ( i == iActiveSlot )
		{
			CBasePlayerWeapon *p = pPlayer->GetFirstPos(iActiveSlot);
			if ( p )
				iWidth = p->GetWeaponInfo()->GetHUDInfo()->GetActive().rect.right - p->GetWeaponInfo()->GetHUDInfo()->GetActive().rect.left;
			else
				iWidth = m_iBucketWidth;
		}
		else
			iWidth = m_iBucketWidth;

		SPR_DrawAdditive(0, x, y, &GetHud().GetSpriteRect(m_HUD_bucket0 + i));
		
		x += iWidth + 5;
	}


	a = 128; //!!!
	x = 10;

	// Draw all of the buckets
	for (i = 0; i < iBucketsToDraw; i++)
	{
		y = m_iBucketHeight + 10;

		// If this is the active slot, draw the bigger pictures,
		// otherwise just draw boxes
		if ( i == iActiveSlot )
		{
			CBasePlayerWeapon *p = pPlayer->m_rgpPlayerItems[ i ];
			int iWidth = m_iBucketWidth;
			if ( p )
				iWidth = p->GetWeaponInfo()->GetHUDInfo()->GetActive().rect.right - p->GetWeaponInfo()->GetHUDInfo()->GetActive().rect.left;

			for( ; p; p = p->m_pNext )
			{
				if ( !p->GetWeaponInfo() )
					continue;

				auto pHUDInfo = p->GetWeaponInfo()->GetHUDInfo();

				GetHud().GetPrimaryColor().UnpackRGB( r,g,b );
			
				// if active, then we must have ammo.

				if ( m_pActiveSel == p )
				{
					SPR_Set( pHUDInfo->GetActive().hSprite, r, g, b );
					SPR_DrawAdditive(0, x, y, &pHUDInfo->GetActive().rect );

					SPR_Set( GetHud().GetSprite(m_HUD_selection), r, g, b );
					SPR_DrawAdditive(0, x, y, &GetHud().GetSpriteRect(m_HUD_selection));
				}
				else
				{
					// Draw Weapon if Red if no ammo

					if ( pPlayer->HasAmmo(p) )
						ScaleColors(r, g, b, 192);
					else
					{
						GetHud().GetEmptyItemColor().UnpackRGB(r,g,b);
						ScaleColors(r, g, b, 128);
					}

					SPR_Set( pHUDInfo->GetInactive().hSprite, r, g, b );
					SPR_DrawAdditive( 0, x, y, &pHUDInfo->GetInactive().rect );
				}

				// Draw Ammo Bar

				DrawAmmoBar(p, x + m_iABWidth/2, y, m_iABWidth, m_iABHeight);
				
				y += pHUDInfo->GetActive().rect.bottom - pHUDInfo->GetActive().rect.top + 5;
			}

			x += iWidth + 5;

		}
		else
		{
			// Draw Row of weapons.

			GetHud().GetPrimaryColor().UnpackRGB(r,g,b);

			for ( CBasePlayerWeapon* p = pPlayer->m_rgpPlayerItems[ i ]; p; p = p->m_pNext )
			{
				if ( !p->GetWeaponInfo() )
					continue;

				if ( pPlayer->HasAmmo(p) )
				{
					GetHud().GetPrimaryColor().UnpackRGB(r,g,b);
					a = 128;
				}
				else
				{
					GetHud().GetEmptyItemColor().UnpackRGB(r,g,b);
					a = 96;
				}

				FillRGBA( x, y, m_iBucketWidth, m_iBucketHeight, r, g, b, a );

				y += m_iBucketHeight + 5;
			}

			x += m_iBucketWidth + 5;
		}
	}	

	return 1;

}

void CHudAmmo::SetCrosshair( HSPRITE hCrosshair, const wrect_t& rect, int r, int g, int b )
{
	m_hCrosshair = hCrosshair;
	m_CrosshairRC = rect;
	m_iR = r;
	m_iG = g;
	m_iB = b;
}

void SetCrosshair( HSPRITE hCrosshair, const wrect_t& crosshairRC, int r, int g, int b )
{
	//gEngfuncs.pfnSetCrosshair( hCrosshair, crosshairRC, r, g, b );

	if( auto pElement = GETHUDCLASS( CHudAmmo ) )
	{
		pElement->SetCrosshair( hCrosshair, crosshairRC, r, g, b );
	}
}
