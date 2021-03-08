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
/*

===== util.cpp ========================================================

  Utility code.  Really not optional after all.

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "SaveRestore.h"
#include <time.h>
#include "shake.h"
#include "Decals.h"
#include "CBasePlayer.h"
#include "Weapons.h"
#include "gamerules/GameRules.h"

#define MAX_MOVEWITH_DEPTH 100

void UTIL_ParametricRocket( CBaseEntity* pEntity, Vector vecOrigin, Vector vecAngles, CBaseEntity* pOwner )
{	
	pEntity->pev->startpos = vecOrigin;
	// Trace out line to end pos
	TraceResult tr;
	UTIL_MakeVectors( vecAngles );
	UTIL_TraceLine( pEntity->pev->startpos, pEntity->pev->startpos + gpGlobals->v_forward * 8192, ignore_monsters, pOwner ? pOwner->edict() : nullptr, &tr);
	pEntity->pev->endpos = tr.vecEndPos;

	// Now compute how long it will take based on current velocity
	Vector vecTravel = pEntity->pev->endpos - pEntity->pev->startpos;
	float travelTime = 0.0;
	if ( pEntity->GetAbsVelocity().Length() > 0 )
	{
		travelTime = vecTravel.Length() / pEntity->GetAbsVelocity().Length();
	}
	pEntity->pev->starttime = gpGlobals->time;
	pEntity->pev->impacttime = gpGlobals->time + travelTime;
}

int g_groupmask = 0;
int g_groupop = 0;

bool g_doingDesired = false;

// Normal overrides
void UTIL_SetGroupTrace( int groupmask, int op )
{
	g_groupmask		= groupmask;
	g_groupop		= op;

	ENGINE_SETGROUPMASK( g_groupmask, g_groupop );
}

void UTIL_UnsetGroupTrace( void )
{
	g_groupmask		= 0;
	g_groupop		= 0;

	ENGINE_SETGROUPMASK( 0, 0 );
}

// Smart version, it'll clean itself up when it pops off stack
UTIL_GroupTrace::UTIL_GroupTrace( int groupmask, int op )
{
	m_oldgroupmask	= g_groupmask;
	m_oldgroupop	= g_groupop;

	g_groupmask		= groupmask;
	g_groupop		= op;

	ENGINE_SETGROUPMASK( g_groupmask, g_groupop );
}

UTIL_GroupTrace::~UTIL_GroupTrace( void )
{
	g_groupmask		=	m_oldgroupmask;
	g_groupop		=	m_oldgroupop;

	ENGINE_SETGROUPMASK( g_groupmask, g_groupop );
}

#ifdef	DEBUG
edict_t *DBG_EntOfVars( const entvars_t *pev )
{
	if (pev->pContainingEntity != NULL)
		return pev->pContainingEntity;
	ALERT(at_console, "entvars_t pContainingEntity is NULL, calling into engine");
	edict_t* pent = (*g_engfuncs.pfnFindEntityByVars)((entvars_t*)pev);
	if (pent == NULL)
		ALERT(at_console, "DAMN!  Even the engine couldn't FindEntityByVars!");
	((entvars_t *)pev)->pContainingEntity = pent;
	return pent;
}
#endif //DEBUG

CBaseEntity* UTIL_FindClientInPVS( const CBaseEntity* const pPVSEntity )
{
	ASSERT( pPVSEntity );

	if( !pPVSEntity )
		return nullptr;

	edict_t* pEnt = FIND_CLIENT_IN_PVS( pPVSEntity->edict() );

	return pEnt ? CBaseEntity::Instance( pEnt ) : nullptr;
}

bool UTIL_GetNextBestWeapon( CBasePlayer *pPlayer, CBasePlayerWeapon *pCurrentWeapon )
{
	return g_pGameRules->GetNextBestWeapon( pPlayer, pCurrentWeapon );
}

int UTIL_EntitiesInBox( CBaseEntity **pList, int listMax, const Vector &mins, const Vector &maxs, int flagMask )
{
	edict_t	*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );

	int count = 0;

	if ( !pEdict )
		return count;

	for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
	{
		if ( pEdict->free )	// Not in use
			continue;
		
		if ( flagMask && !(pEdict->v.flags & flagMask) )	// Does it meet the criteria?
			continue;

		if ( mins.x > pEdict->v.absmax.x ||
			 mins.y > pEdict->v.absmax.y ||
			 mins.z > pEdict->v.absmax.z ||
			 maxs.x < pEdict->v.absmin.x ||
			 maxs.y < pEdict->v.absmin.y ||
			 maxs.z < pEdict->v.absmin.z )
			 continue;

		CBaseEntity* pEntity = CBaseEntity::Instance(pEdict);
		if ( !pEntity )
			continue;

		pList[ count ] = pEntity;
		count++;

		if ( count >= listMax )
			return count;
	}

	return count;
}


int UTIL_MonstersInSphere( CBaseEntity **pList, int listMax, const Vector &center, float radius )
{
	edict_t	*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );

	int count = 0;
	float radiusSquared = radius * radius;

	if ( !pEdict )
		return count;

	for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
	{
		if ( pEdict->free )	// Not in use
			continue;
		
		if ( !(pEdict->v.flags & (FL_CLIENT|FL_MONSTER)) )	// Not a client/monster ?
			continue;

		// Use origin for X & Y since they are centered for all monsters
		// Now X
		float delta = center.x - pEdict->v.origin.x;//(pEdict->v.absmin.x + pEdict->v.absmax.x)*0.5;
		delta *= delta;

		if ( delta > radiusSquared )
			continue;
		float distance = delta;
		
		// Now Y
		delta = center.y - pEdict->v.origin.y;//(pEdict->v.absmin.y + pEdict->v.absmax.y)*0.5;
		delta *= delta;

		distance += delta;
		if ( distance > radiusSquared )
			continue;

		// Now Z
		delta = center.z - (pEdict->v.absmin.z + pEdict->v.absmax.z)*0.5;
		delta *= delta;

		distance += delta;
		if ( distance > radiusSquared )
			continue;

		CBaseEntity* pEntity = CBaseEntity::Instance(pEdict);
		if ( !pEntity )
			continue;

		pList[ count ] = pEntity;
		count++;

		if ( count >= listMax )
			return count;
	}


	return count;
}

CBaseEntity *UTIL_FindEntityInSphere( CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius )
{
	edict_t	*pentEntity;

	if (pStartEntity)
		pentEntity = pStartEntity->edict();
	else
		pentEntity = nullptr;

	pentEntity = FIND_ENTITY_IN_SPHERE( pentEntity, vecCenter, flRadius);

	if (!FNullEnt(pentEntity))
		return CBaseEntity::Instance(pentEntity);
	return nullptr;
}


CBaseEntity *UTIL_FindEntityByString( CBaseEntity *pStartEntity, const char *szKeyword, const char *szValue )
{
	edict_t* pentEntity;

	if (pStartEntity)
		pentEntity = pStartEntity->edict();
	else
		pentEntity = NULL;

	for (;;)
	{
		// Don't change this to use UTIL_FindEntityByString!
		pentEntity = FIND_ENTITY_BY_STRING(pentEntity, szKeyword, szValue);

		// if pentEntity (the edict) is null, we're at the end of the entities. Give up.
		if (FNullEnt(pentEntity))
			return NULL;

		// ...but if only pEntity (the classptr) is null, we've just got one dud, so we try again.
		CBaseEntity* pEntity = CBaseEntity::Instance(pentEntity);
		if (pEntity)
			return pEntity;
	}
}

CBaseEntity *UTIL_FindEntityByClassname( CBaseEntity *pStartEntity, const char *szName )
{
	return UTIL_FindEntityByString( pStartEntity, "classname", szName );
}

CBaseEntity* UTIL_FindEntityByTargetname(CBaseEntity* pStartEntity, const char* szName, CBaseEntity* pActivator)
{
	if (FStrEq(szName, "*locus"))
	{
		if (pActivator && (pStartEntity == nullptr || pActivator->eoffset() > pStartEntity->eoffset()))
			return pActivator;

		return nullptr;
	}

	return UTIL_FindEntityByTargetname(pStartEntity, szName);
}

CBaseEntity* UTIL_FindEntityByTargetname(CBaseEntity* pStartEntity, const char* szName)
{
	CBaseEntity* pFound = UTIL_FollowReference(pStartEntity, szName);
	if (pFound)
		return pFound;

	return UTIL_FindEntityByString(pStartEntity, "targetname", szName);
}

CBaseEntity *UTIL_FindEntityGeneric( const char *szWhatever, const Vector &vecSrc, float flRadius )
{
	CBaseEntity* pEntity = UTIL_FindEntityByTargetname(nullptr, szWhatever);
	if (pEntity)
		return pEntity;

	CBaseEntity *pSearch = NULL;
	float flMaxDist2 = flRadius * flRadius;
	while ((pSearch = UTIL_FindEntityByClassname( pSearch, szWhatever )) != NULL)
	{
		float flDist2 = (pSearch->GetAbsOrigin() - vecSrc).Length();
		flDist2 = flDist2 * flDist2;
		if (flMaxDist2 > flDist2)
		{
			pEntity = pSearch;
			flMaxDist2 = flDist2;
		}
	}
	return pEntity;
}

CBaseEntity* UTIL_FindEntityByTarget( CBaseEntity* pStartEntity, const char* const pszTarget )
{
	return UTIL_FindEntityByString( pStartEntity, "target", pszTarget );
}

CBaseEntity* UTIL_EntityByIndex( const int iIndex )
{
	if( auto pEntity = g_engfuncs.pfnPEntityOfEntIndex( iIndex ) )
		return GET_PRIVATE( pEntity );

	return nullptr;
}

// returns a CBasePlayer pointer to a player by index.  Only returns if the player is spawned and connected
// otherwise returns nullptr
// Index is 1 based
CBasePlayer	*UTIL_PlayerByIndex( int playerIndex )
{
	CBasePlayer *pPlayer = nullptr;

	if ( playerIndex > 0 && playerIndex <= gpGlobals->maxClients )
	{
		edict_t *pPlayerEdict = INDEXENT( playerIndex );
		if ( pPlayerEdict && !pPlayerEdict->free )
		{
			pPlayer = static_cast<CBasePlayer*>( CBaseEntity::Instance( pPlayerEdict ) );
		}
	}
	
	return pPlayer;
}

CBasePlayer* UTIL_FindPlayerByName( const char *pszTestName )
{
	for( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		edict_t *pEdict = g_engfuncs.pfnPEntityOfEntIndex( i );
		if( pEdict )
		{
			CBaseEntity *pEnt = CBaseEntity::Instance( pEdict );
			if( pEnt && pEnt->IsPlayer() )
			{
				if( stricmp( pEnt->GetNetName(), pszTestName ) == 0 )
				{
					return ( CBasePlayer* ) pEnt;
				}
			}
		}
	}

	return nullptr;
}

static unsigned short FixedUnsigned16( float value, float scale )
{
	int output;

	output = value * scale;
	if ( output < 0 )
		output = 0;
	if ( output > 0xFFFF )
		output = 0xFFFF;

	return (unsigned short)output;
}

static short FixedSigned16( float value, float scale )
{
	int output;

	output = value * scale;

	if ( output > 32767 )
		output = 32767;

	if ( output < -32768 )
		output = -32768;

	return (short)output;
}

// Shake the screen of all clients within radius
// radius == 0, shake all clients
// UNDONE: Allow caller to shake clients not ONGROUND?
// UNDONE: Fix falloff model (disabled)?
// UNDONE: Affect user controls?
void UTIL_ScreenShake( const Vector &center, float amplitude, float frequency, float duration, float radius )
{
	int			i;
	float		localAmplitude;
	ScreenShake	shake;

	shake.duration = FixedUnsigned16( duration, 1<<12 );		// 4.12 fixed
	shake.frequency = FixedUnsigned16( frequency, 1<<8 );	// 8.8 fixed

	for ( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );

		if ( !pPlayer || !pPlayer->GetFlags().Any( FL_ONGROUND ) )	// Don't shake if not onground
			continue;

		localAmplitude = 0;

		if ( radius <= 0 )
			localAmplitude = amplitude;
		else
		{
			Vector delta = center - pPlayer->GetAbsOrigin();
			float distance = delta.Length();
	
			// Had to get rid of this falloff - it didn't work well
			if ( distance < radius )
				localAmplitude = amplitude;//radius - distance;
		}
		if ( localAmplitude )
		{
			shake.amplitude = FixedUnsigned16( localAmplitude, 1<<12 );		// 4.12 fixed
			
			MESSAGE_BEGIN( MSG_ONE, gmsgShake, NULL, pPlayer );		// use the magic #1 for "one client"
				
				WRITE_SHORT( shake.amplitude );				// shake amount
				WRITE_SHORT( shake.duration );				// shake lasts this long
				WRITE_SHORT( shake.frequency );				// shake noise frequency

			MESSAGE_END();
		}
	}
}



void UTIL_ScreenShakeAll( const Vector &center, float amplitude, float frequency, float duration )
{
	UTIL_ScreenShake( center, amplitude, frequency, duration, 0 );
}


void UTIL_ScreenFadeBuild( ScreenFade &fade, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
	fade.duration = FixedUnsigned16( fadeTime, 1<<12 );		// 4.12 fixed
	fade.holdTime = FixedUnsigned16( fadeHold, 1<<12 );		// 4.12 fixed
	fade.r = (int)color.x;
	fade.g = (int)color.y;
	fade.b = (int)color.z;
	fade.a = alpha;
	fade.fadeFlags = flags;
}


void UTIL_ScreenFadeWrite( const ScreenFade &fade, CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsNetClient() )
		return;

	MESSAGE_BEGIN( MSG_ONE, gmsgFade, NULL, pEntity );		// use the magic #1 for "one client"
		
		WRITE_SHORT( fade.duration );		// fade lasts this long
		WRITE_SHORT( fade.holdTime );		// fade lasts this long
		WRITE_SHORT( fade.fadeFlags );		// fade type (in / out)
		WRITE_BYTE( fade.r );				// fade red
		WRITE_BYTE( fade.g );				// fade green
		WRITE_BYTE( fade.b );				// fade blue
		WRITE_BYTE( fade.a );				// fade blue

	MESSAGE_END();
}


void UTIL_ScreenFadeAll( const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
	int			i;
	ScreenFade	fade;


	UTIL_ScreenFadeBuild( fade, color, fadeTime, fadeHold, alpha, flags );

	for ( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
	
		UTIL_ScreenFadeWrite( fade, pPlayer );
	}
}


void UTIL_ScreenFade( CBaseEntity *pEntity, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
	ScreenFade	fade;

	UTIL_ScreenFadeBuild( fade, color, fadeTime, fadeHold, alpha, flags );
	UTIL_ScreenFadeWrite( fade, pEntity );
}


void UTIL_HudMessage( CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage )
{
	if ( !pEntity || !pEntity->IsNetClient() )
		return;

	MESSAGE_BEGIN( MSG_ONE, SVC_TEMPENTITY, NULL, pEntity );
		WRITE_BYTE( TE_TEXTMESSAGE );
		WRITE_BYTE( textparms.channel & 0xFF );

		WRITE_SHORT( FixedSigned16( textparms.x, 1<<13 ) );
		WRITE_SHORT( FixedSigned16( textparms.y, 1<<13 ) );
		WRITE_BYTE( textparms.effect );

		WRITE_BYTE( textparms.r1 );
		WRITE_BYTE( textparms.g1 );
		WRITE_BYTE( textparms.b1 );
		WRITE_BYTE( textparms.a1 );

		WRITE_BYTE( textparms.r2 );
		WRITE_BYTE( textparms.g2 );
		WRITE_BYTE( textparms.b2 );
		WRITE_BYTE( textparms.a2 );

		WRITE_SHORT( FixedUnsigned16( textparms.fadeinTime, 1<<8 ) );
		WRITE_SHORT( FixedUnsigned16( textparms.fadeoutTime, 1<<8 ) );
		WRITE_SHORT( FixedUnsigned16( textparms.holdTime, 1<<8 ) );

		if ( textparms.effect == 2 )
			WRITE_SHORT( FixedUnsigned16( textparms.fxTime, 1<<8 ) );
		
		if ( strlen( pMessage ) < 512 )
		{
			WRITE_STRING( pMessage );
		}
		else
		{
			char tmp[512];
			strncpy( tmp, pMessage, 511 );
			tmp[511] = 0;
			WRITE_STRING( tmp );
		}
	MESSAGE_END();
}

void UTIL_HudMessageAll( const hudtextparms_t &textparms, const char *pMessage )
{
	int			i;

	for ( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
		if ( pPlayer )
			UTIL_HudMessage( pPlayer, textparms, pMessage );
	}
}

void UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgTextMsg );
		WRITE_BYTE( msg_dest );
		WRITE_STRING( msg_name );

		if ( param1 )
			WRITE_STRING( param1 );
		if ( param2 )
			WRITE_STRING( param2 );
		if ( param3 )
			WRITE_STRING( param3 );
		if ( param4 )
			WRITE_STRING( param4 );

	MESSAGE_END();
}

void ClientPrint( CBaseEntity* pClient, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	MESSAGE_BEGIN( MSG_ONE, gmsgTextMsg, NULL, pClient );
		WRITE_BYTE( msg_dest );
		WRITE_STRING( msg_name );

		if ( param1 )
			WRITE_STRING( param1 );
		if ( param2 )
			WRITE_STRING( param2 );
		if ( param3 )
			WRITE_STRING( param3 );
		if ( param4 )
			WRITE_STRING( param4 );

	MESSAGE_END();
}

char* UTIL_dtos(const int iValue)
{
	//This buffer size calculation determines the number of characters needed for an int, plus a null terminator.
	//See http://stackoverflow.com/questions/3919995/determining-sprintf-buffer-size-whats-the-standard/3920025#3920025
	//The old buffer size used by the SDK functions was 8.
	static char szBuffers[NUM_STATIC_BUFFERS][(((sizeof(int) * CHAR_BIT) / 3) + 3) + 1];
	static size_t uiBufferIndex = 0;
	uiBufferIndex = (uiBufferIndex + 1) % NUM_STATIC_BUFFERS;
	snprintf(szBuffers[uiBufferIndex], sizeof(szBuffers[uiBufferIndex]), "%d", iValue);
	return szBuffers[uiBufferIndex];
}

void UTIL_SayText( const char *pText, CBaseEntity *pEntity )
{
	if ( !pEntity->IsNetClient() )
		return;

	MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, pEntity );
		WRITE_BYTE( pEntity->entindex() );
		WRITE_STRING( pText );
	MESSAGE_END();
}

void UTIL_SayTextAll( const char *pText, CBaseEntity *pEntity )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
		WRITE_BYTE( pEntity->entindex() );
		WRITE_STRING( pText );
	MESSAGE_END();
}

void UTIL_ShowMessage( const char *pString, CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsNetClient() )
		return;

	MESSAGE_BEGIN( MSG_ONE, gmsgHudText, NULL, pEntity );
	WRITE_STRING( pString );
	MESSAGE_END();
}

void UTIL_ShowMessageAll( const char *pString )
{
	// loop through all players
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
		if ( pPlayer )
			UTIL_ShowMessage( pString, pPlayer );
	}
}

// Overloaded to add IGNORE_GLASS
void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_LINE( vecStart, vecEnd, ( igmon == ignore_monsters ? TRF_IGNORE_MONSTERS : TRF_NONE ) | ( ignoreGlass ? TRF_IGNORE_GLASS : 0 ), pentIgnore, ptr );
}

void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_LINE( vecStart, vecEnd, ( igmon == ignore_monsters ? TRF_IGNORE_MONSTERS : TRF_NONE ), pentIgnore, ptr );
}

void UTIL_TraceMonsterHull(CBaseEntity* pEntity, const Vector& v1, const Vector& v2, IGNORE_MONSTERS igmon, CBaseEntity* pentToSkip, TraceResult& tr)
{
	TRACE_MONSTER_HULL(pEntity->edict(), v1, v2, (igmon == ignore_monsters ? TRF_IGNORE_MONSTERS : TRF_NONE), pentToSkip ? pentToSkip->edict() : nullptr, &tr);
}

void UTIL_TraceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, const Hull::Hull hullNumber, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_HULL( vecStart, vecEnd, ( igmon == ignore_monsters ? TRF_IGNORE_MONSTERS : TRF_NONE ), static_cast<int>( hullNumber ), pentIgnore, ptr );
}

void UTIL_TraceModel( const Vector &vecStart, const Vector &vecEnd, const Hull::Hull hullNumber, edict_t *pentModel, TraceResult *ptr )
{
	g_engfuncs.pfnTraceModel( vecStart, vecEnd, static_cast<int>( hullNumber ), pentModel, ptr );
}

TraceResult UTIL_GetGlobalTrace()
{
	TraceResult tr;

	tr.fAllSolid = gpGlobals->trace_allsolid;
	tr.fStartSolid = gpGlobals->trace_startsolid;
	tr.fInOpen = gpGlobals->trace_inopen;
	tr.fInWater = gpGlobals->trace_inwater;
	tr.flFraction = gpGlobals->trace_fraction;
	tr.flPlaneDist = gpGlobals->trace_plane_dist;
	tr.pHit = gpGlobals->trace_ent;
	tr.vecEndPos = gpGlobals->trace_endpos;
	tr.vecPlaneNormal = gpGlobals->trace_plane_normal;
	tr.iHitgroup = gpGlobals->trace_hitgroup;
	return tr;
}

void UTIL_SetEdictOrigin(edict_t* pEdict, const Vector& vecOrigin)
{
	if (pEdict)
		SET_ORIGIN(pEdict, vecOrigin);
}
	
void UTIL_SetSize( CBaseEntity* pEntity, const Vector& vecMin, const Vector& vecMax )
{
	SET_SIZE( pEntity->edict(), vecMin, vecMax );
}
	
void UTIL_SetOrigin( CBaseEntity* pEntity, const Vector& vecOrigin )
{
	if ( auto pEnt = pEntity->edict() )
		UTIL_SetEdictOrigin( pEnt, vecOrigin );
}

void UTIL_ParticleEffect( const Vector &vecOrigin, const Vector &vecDirection, const unsigned int ulColor, const unsigned int ulCount )
{
	PARTICLE_EFFECT( vecOrigin, vecDirection, (float)ulColor, (float)ulCount );
}
	
Vector UTIL_GetAimVector( const CBaseEntity* const pEntity, const float flSpeed )
{
	Vector tmp;
	GET_AIM_VECTOR( const_cast<edict_t*>( pEntity->edict() ), flSpeed, tmp );
	return tmp;
}

bool UTIL_IsMasterTriggered( string_t sMaster, const CBaseEntity* const pActivator )
{
	if( sMaster )
	{
		CBaseEntity* pTarget = UTIL_FindEntityByTargetname( nullptr, STRING( sMaster ) );
	
		if( !FNullEnt( pTarget ) )
		{
			if( pTarget->ObjectCaps() & FCAP_MASTER )
				return pTarget->IsTriggered( pActivator );
		}

		ALERT( at_console, "Master was null or not a master!\n" );
	}

	// if this isn't a master entity, just say yes.
	return true;
}

bool UTIL_ShouldShowBlood( int color )
{
	if ( color != DONT_BLEED )
	{
		if ( color == BLOOD_COLOR_RED )
		{
			if ( CVAR_GET_FLOAT("violence_hblood") != 0 )
				return true;
		}
		else
		{
			if ( CVAR_GET_FLOAT("violence_ablood") != 0 )
				return true;
		}
	}
	return false;
}

void UTIL_BloodStream( const Vector &origin, const Vector &direction, int color, int amount )
{
	if ( !UTIL_ShouldShowBlood( color ) )
		return;

	if ( g_Language == LANGUAGE_GERMAN && color == BLOOD_COLOR_RED )
		color = 0;

	
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, origin );
		WRITE_BYTE( TE_BLOODSTREAM );
		WRITE_COORD( origin.x );
		WRITE_COORD( origin.y );
		WRITE_COORD( origin.z );
		WRITE_COORD( direction.x );
		WRITE_COORD( direction.y );
		WRITE_COORD( direction.z );
		WRITE_BYTE( color );
		WRITE_BYTE( min( amount, 255 ) );
	MESSAGE_END();
}				

void UTIL_BloodDrips( const Vector &origin, const Vector &direction, int color, int amount )
{
	if ( !UTIL_ShouldShowBlood( color ) )
		return;

	if ( color == DONT_BLEED || amount == 0 )
		return;

	if ( g_Language == LANGUAGE_GERMAN && color == BLOOD_COLOR_RED )
		color = 0;

	if ( g_pGameRules->IsMultiplayer() )
	{
		// scale up blood effect in multiplayer for better visibility
		amount *= 2;
	}

	if ( amount > 255 )
		amount = 255;

	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, origin );
		WRITE_BYTE( TE_BLOODSPRITE );
		WRITE_COORD( origin.x);								// pos
		WRITE_COORD( origin.y);
		WRITE_COORD( origin.z);
		WRITE_SHORT( g_sModelIndexBloodSpray );				// initial sprite model
		WRITE_SHORT( g_sModelIndexBloodDrop );				// droplet sprite models
		WRITE_BYTE( color );								// color index into host_basepal
		WRITE_BYTE( min( max( 3, amount / 10 ), 16 ) );		// size
	MESSAGE_END();
}				

Vector UTIL_RandomBloodVector( void )
{
	Vector direction;

	direction.x = RANDOM_FLOAT ( -1, 1 );
	direction.y = RANDOM_FLOAT ( -1, 1 );
	direction.z = RANDOM_FLOAT ( 0, 1 );

	return direction;
}


void UTIL_BloodDecalTrace( TraceResult *pTrace, int bloodColor )
{
	if ( UTIL_ShouldShowBlood( bloodColor ) )
	{
		if ( bloodColor == BLOOD_COLOR_RED )
			UTIL_DecalTrace( pTrace, DECAL_BLOOD1 + RANDOM_LONG(0,5) );
		else
			UTIL_DecalTrace( pTrace, DECAL_YBLOOD1 + RANDOM_LONG(0,5) );
	}
}


void UTIL_DecalTrace( TraceResult *pTrace, int decalNumber )
{
	short entityIndex;
	int index;
	int message;

	if ( decalNumber < 0 )
		return;

	index = gDecals[ decalNumber ].index;

	if ( index < 0 )
		return;

	if (pTrace->flFraction == 1.0)
		return;

	// Only decal BSP models
	if ( pTrace->pHit )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( pTrace->pHit );
		if ( pEntity && !pEntity->IsBSPModel() )
			return;
		entityIndex = ENTINDEX( pTrace->pHit );
	}
	else 
		entityIndex = 0;

	message = TE_DECAL;
	if ( entityIndex != 0 )
	{
		if ( index > 255 )
		{
			message = TE_DECALHIGH;
			index -= 256;
		}
	}
	else
	{
		message = TE_WORLDDECAL;
		if ( index > 255 )
		{
			message = TE_WORLDDECALHIGH;
			index -= 256;
		}
	}
	
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( message );
		WRITE_COORD( pTrace->vecEndPos.x );
		WRITE_COORD( pTrace->vecEndPos.y );
		WRITE_COORD( pTrace->vecEndPos.z );
		WRITE_BYTE( index );
		if ( entityIndex )
			WRITE_SHORT( entityIndex );
	MESSAGE_END();
}

/*
==============
UTIL_PlayerDecalTrace

A player is trying to apply his custom decal for the spray can.
Tell connected clients to display it, or use the default spray can decal
if the custom can't be loaded.
==============
*/
void UTIL_PlayerDecalTrace( TraceResult *pTrace, int playernum, int decalNumber, const bool bIsCustom )
{
	int index;
	
	if (!bIsCustom)
	{
		if ( decalNumber < 0 )
			return;

		index = gDecals[ decalNumber ].index;
		if ( index < 0 )
			return;
	}
	else
		index = decalNumber;

	if (pTrace->flFraction == 1.0)
		return;

	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_PLAYERDECAL );
		WRITE_BYTE ( playernum );
		WRITE_COORD( pTrace->vecEndPos.x );
		WRITE_COORD( pTrace->vecEndPos.y );
		WRITE_COORD( pTrace->vecEndPos.z );
		WRITE_SHORT( (short)ENTINDEX(pTrace->pHit) );
		WRITE_BYTE( index );
	MESSAGE_END();
}

void UTIL_GunshotDecalTrace( TraceResult *pTrace, int decalNumber )
{
	//Prevent array index out of bounds here - Solokiller
	if ( decalNumber < 0 || static_cast<size_t>( decalNumber ) >= gDecalsSize )
		return;

	int index = gDecals[ decalNumber ].index;
	if ( index < 0 )
		return;

	if (pTrace->flFraction == 1.0)
		return;

	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pTrace->vecEndPos );
		WRITE_BYTE( TE_GUNSHOTDECAL );
		WRITE_COORD( pTrace->vecEndPos.x );
		WRITE_COORD( pTrace->vecEndPos.y );
		WRITE_COORD( pTrace->vecEndPos.z );
		WRITE_SHORT( (short)ENTINDEX(pTrace->pHit) );
		WRITE_BYTE( index );
	MESSAGE_END();
}

//
// Makes flagged buttons spark when turned off
//
void DoSpark( CBaseEntity* pEntity, const Vector &location )
{
	Vector tmp = location + pEntity->GetBounds() * 0.5;
	UTIL_Sparks( tmp );

	float flVolume = RANDOM_FLOAT( 0.25, 0.75 ) * 0.4;//random volume range
	switch( ( int ) ( RANDOM_FLOAT( 0, 1 ) * 6 ) )
	{
	case 0: EMIT_SOUND( pEntity, CHAN_VOICE, "buttons/spark1.wav", flVolume, ATTN_NORM ); break;
	case 1: EMIT_SOUND( pEntity, CHAN_VOICE, "buttons/spark2.wav", flVolume, ATTN_NORM ); break;
	case 2: EMIT_SOUND( pEntity, CHAN_VOICE, "buttons/spark3.wav", flVolume, ATTN_NORM ); break;
	case 3: EMIT_SOUND( pEntity, CHAN_VOICE, "buttons/spark4.wav", flVolume, ATTN_NORM ); break;
	case 4: EMIT_SOUND( pEntity, CHAN_VOICE, "buttons/spark5.wav", flVolume, ATTN_NORM ); break;
	case 5: EMIT_SOUND( pEntity, CHAN_VOICE, "buttons/spark6.wav", flVolume, ATTN_NORM ); break;
	}
}

bool UTIL_TeamsMatch( const char *pTeamName1, const char *pTeamName2 )
{
	// Everyone matches unless it's teamplay
	if ( !g_pGameRules->IsTeamplay() )
		return true;

	// Both on a team?
	if ( *pTeamName1 != 0 && *pTeamName2 != 0 )
	{
		if ( !stricmp( pTeamName1, pTeamName2 ) )	// Same Team?
			return true;
	}

	return false;
}

bool UTIL_IsValidEntity( const CBaseEntity* const pEntity )
{
	if ( !pEntity || pEntity->edict()->free || pEntity->GetFlags().Any( FL_KILLME ) )
		return false;
	return true;
}

CBaseEntity* UTIL_CreateNamedEntity( const char* const pszClassName )
{
	ASSERT( pszClassName );

	edict_t* pEdict = CREATE_NAMED_ENTITY( MAKE_STRING( pszClassName ) );

	if( FNullEnt( pEdict ) )
	{
		return nullptr;
	}

	if( auto pEntity = CBaseEntity::Instance( &pEdict->v ) )
		return pEntity;

	ALERT( at_console, "UTIL_CreateNamedEntity: Couldn't get CBaseEntity instance for \"%s\"\n", pszClassName );

	REMOVE_ENTITY( pEdict );

	return nullptr;
}

void UTIL_PrecacheOther( const char *szClassname )
{
	edict_t* pent = CREATE_NAMED_ENTITY(MAKE_STRING(szClassname));
	if (FNullEnt(pent))
	{
		ALERT(at_console, "NULL Ent in UTIL_PrecacheOther\n");
		return;
	}

	CBaseEntity* pEntity = CBaseEntity::Instance(VARS(pent));
	if (pEntity)
		pEntity->Precache();

	UTIL_RemoveNow( pEntity );
}

//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf( const char* const pszFormat, ... )
{
	va_list			argptr;
	static char		string[1024];
	
	va_start ( argptr, pszFormat );
	vsprintf ( string, pszFormat, argptr );
	va_end   ( argptr );

	UTIL_Log(pszFormat);

	// Print to server console
	ALERT( at_logged, "%s", string );
}

void UTIL_ServerPrintf( const char* const pszFormat, ... )
{
	va_list			argptr;
	static char		string[ 1024 ];

	va_start( argptr, pszFormat );
	vsprintf( string, pszFormat, argptr );
	va_end( argptr );

	UTIL_Log(pszFormat);
	
	// Print to server console
	g_engfuncs.pfnServerPrint( string );
}

void UTIL_Log(const char* fmt, ...)
{
	va_list ap;
	static char string[1024];

	va_start(ap, fmt);
	vsnprintf(string, sizeof(string), fmt, ap);
	va_end(ap);

	if (strlen(string) < sizeof(string) - 2)
		strcat(string, "\n");
	else
		string[strlen(string) - 1] = '\n';

	FILE* fp = fopen("spirit.log", "at");
	if (fp)
	{
		fprintf(fp, "%s", string);
		fclose(fp);
	}
}

//Newer compilers might define _rotr as a macro. - Solokiller
#if !defined( _WIN32 ) && !defined( _rotr )
extern "C" {
unsigned _rotr ( unsigned val, int shift)
{
        /*register*/ unsigned lobit;        /* non-zero means lo bit set */
        /*register*/ unsigned num = val;    /* number to rotate */

        shift &= 0x1f;                  /* modulo 32 -- this will also make
                                           negative shifts work */

        while (shift--) {
                lobit = num & 1;        /* get high bit */
                num >>= 1;              /* shift right one bit */
                if (lobit)
                        num |= 0x80000000;  /* set hi bit if lo bit was set */
        }

        return num;
}
}
#endif

bool UTIL_SetTypeDescValue( void* pEntity, const TYPEDESCRIPTION& desc, const char* const pszValue )
{
	switch( desc.fieldType )
	{
	case FIELD_MODELNAME:
	case FIELD_SOUNDNAME:
	case FIELD_STRING:
		( *( int * ) ( ( char * ) pEntity + desc.fieldOffset ) ) = ALLOC_STRING( pszValue );
		break;

	case FIELD_TIME:
	case FIELD_FLOAT:
		( *( float * ) ( ( char * ) pEntity + desc.fieldOffset ) ) = atof( pszValue );
		break;

	case FIELD_INTEGER:
		( *( int * ) ( ( char * ) pEntity + desc.fieldOffset ) ) = atoi( pszValue );
		break;

	case FIELD_POSITION_VECTOR:
	case FIELD_VECTOR:
		UTIL_StringToVector( *reinterpret_cast<Vector*>( ( ( char * ) pEntity + desc.fieldOffset ) ), pszValue );
		break;

	default:
	case FIELD_EVARS:
	case FIELD_CLASSPTR:
	case FIELD_EDICT:
	case FIELD_ENTITY:
		return false;
	}

	return true;
}

void EntvarsKeyvalue( entvars_t *pev, KeyValueData *pkvd )
{
	for ( size_t i = 0; i < gEntvarsDataMap.uiNumDescriptors; i++ )
	{
		const TYPEDESCRIPTION* pField = &gEntvarsDataMap.pTypeDesc[i];

		if ( !stricmp( pField->fieldName, pkvd->szKeyName ) )
		{
			if( !UTIL_SetTypeDescValue( pev, *pField, pkvd->szValue ) )
				ALERT( at_error, "Bad field in entity!!\n" );

			pkvd->fHandled = true;
			return;
		}
	}
}

CBaseEntity* UTIL_RandomTargetname( const char* const pszName )
{
	int total = 0;

	CBaseEntity *pEntity = nullptr;
	CBaseEntity *pNewEntity = nullptr;

	//Enumerate all entities with the given name and assign it to pEntity if a random check succeeds.
	while( ( pNewEntity = UTIL_FindEntityByTargetname( pNewEntity, pszName ) ) != nullptr )
	{
		++total;

		if( RANDOM_LONG( 0, total - 1 ) < 1 )
			pEntity = pNewEntity;
	}

	return pEntity;
}

CBaseEntity* UTIL_RandomClassname( const char* pszName )
{
	int total = 0;

	CBaseEntity *pEntity = nullptr;
	CBaseEntity *pNewEntity = nullptr;

	while( ( pNewEntity = UTIL_FindEntityByClassname( pNewEntity, pszName ) ) != nullptr )
	{
		++total;

		if( RANDOM_LONG( 0, total - 1 ) < 1 )
			pEntity = pNewEntity;
	}

	return pEntity;
}

//
// BModelOrigin - calculates origin of a bmodel from absmin/size because all bmodel origins are 0 0 0
//
Vector VecBModelOrigin( const CBaseEntity* const pBModel )
{
	return pBModel->GetAbsMin() + ( pBModel->GetBounds() * 0.5 );
}

// Initialize absmin & absmax to the appropriate box
void SetObjectCollisionBox( entvars_t *pev )
{
	if( ( pev->solid == SOLID_BSP ) &&
		( pev->angles.x || pev->angles.y || pev->angles.z ) )
	{	// expand for rotation
		int	i;

		float max = 0;
		for( i = 0; i<3; i++ )
		{
			float v = fabs(pev->mins[i]);
			if( v > max )
				max = v;
			v = fabs( pev->maxs[ i ] );
			if( v > max )
				max = v;
		}
		for( i = 0; i<3; i++ )
		{
			pev->absmin[ i ] = pev->origin[ i ] - max;
			pev->absmax[ i ] = pev->origin[ i ] + max;
		}
	}
	else
	{
		pev->absmin = pev->origin + pev->mins;
		pev->absmax = pev->origin + pev->maxs;
	}

	pev->absmin.x -= 1;
	pev->absmin.y -= 1;
	pev->absmin.z -= 1;
	pev->absmax.x += 1;
	pev->absmax.y += 1;
	pev->absmax.z += 1;
}

CBaseEntity* UTIL_FindEntityForward( CBaseEntity* pMe )
{
	TraceResult tr;

	UTIL_MakeVectors( pMe->GetViewAngle() );
	UTIL_TraceLine( pMe->GetAbsOrigin() + pMe->GetViewOffset(), pMe->GetAbsOrigin() + pMe->GetViewOffset() + gpGlobals->v_forward * 8192, dont_ignore_monsters, pMe->edict(), &tr );
	if( tr.flFraction != 1.0 && !FNullEnt( tr.pHit ) )
	{
		CBaseEntity *pHit = CBaseEntity::Instance( tr.pHit );
		return pHit;
	}
	return nullptr;
}

CBaseEntity* UTIL_FindEntitiesInPVS( CBaseEntity* pEntity )
{
	if( !pEntity )
		return nullptr;

	auto pResult = g_engfuncs.pfnEntitiesInPVS( pEntity->edict() );

	if( pResult )
		return GET_PRIVATE( pResult );

	return nullptr;
}

void UTIL_MakeStatic( CBaseEntity* pEntity )
{
	if( !pEntity )
		return;

	g_engfuncs.pfnMakeStatic( pEntity->edict() );
}

bool UTIL_EntIsOnFloor( CBaseEntity* pEntity )
{
	if( !pEntity )
		return false;

	return !!g_engfuncs.pfnEntIsOnFloor( pEntity->edict() );
}

const texture_t* UTIL_TraceTexture( CBaseEntity* pEntity, const Vector& vecStart, const Vector& vecEnd )
{
	return TRACE_TEXTURE( pEntity ? pEntity->edict() : nullptr, vecStart, vecEnd );
}

DropToFloor UTIL_DropToFloor( CBaseEntity* pEntity )
{
	ASSERT( pEntity );

	return static_cast<DropToFloor>( DROP_TO_FLOOR( pEntity->edict() ) );
}

bool UTIL_WalkMove( CBaseEntity* pEntity, float yaw, float dist, WalkMove mode )
{
	return WALK_MOVE( pEntity->edict(), yaw, dist, mode ) != 0;
}

int UTIL_GetPlayerUserId( CBaseEntity* pEntity )
{
	ASSERT( pEntity );

	return GETPLAYERUSERID( pEntity->edict() );
}

const char* UTIL_GetPlayerAuthId( const CBaseEntity* pEntity )
{
	ASSERT( pEntity );

	return GETPLAYERAUTHID( pEntity->edict() );
}

bool UTIL_IsBrushEntity( const CBaseEntity* const pEnt )
{
	//worldspawn has index 1, not 0. 0 is used to indicate "no model", and is an empty string.
	if( !pEnt || pEnt->GetModelIndex() == 0 )
		return false;

	//The name of worldspawn's model is maps/<mapname>.bsp, so account for that case.
	if( pEnt->GetModelIndex() == CWorld::WORLD_MODELINDEX )
		return true;

	const char* pszName = pEnt->GetModelName();

	if( !pszName || !( *pszName ) )
		return false;

	if( *pszName != '*' )
		return false;

	++pszName;

	if( *pszName == '\0' )
		return false;

	char* pszEnd;

	const auto index = strtoul( pszName, &pszEnd, 10 );

	if( *pszEnd != '\0' )
		return false;

	//The index of the model must match the number in the model name string + 1.
	//This is oddly numbered because the world has index 1, but the numbers start at 1 for submodels.
	//This means that the first brush entity has index 2, and name "*1".
	return pEnt->GetModelIndex() == static_cast<int>( index + 1 );
}

bool UTIL_IsPointEntity( const CBaseEntity* const pEnt )
{
	if( !pEnt )
		return false;

	if( !pEnt->GetModelIndex() )
		return true;

	//Keep the special cases for these just in case of weird edge cases. - Solokiller
	if( pEnt->ClassnameIs( "info_target" ) || pEnt->ClassnameIs( "info_landmark" ) || pEnt->ClassnameIs( "path_corner" ) )
		return true;

	return !UTIL_IsBrushEntity( pEnt );
}

void Cvar_DirectSet( cvar_t* pCvar, const float flValue )
{
	g_engfuncs.pfnCvar_DirectSet( pCvar, UTIL_VarArgs( "%f", flValue ) );
}

char* COM_FileExtension(const char* in)
{
	static char exten[8];
	int i;

	while (*in && *in != '.')
		in++;
	if (!*in)
		return "";
	in++;
	for (i = 0; i < 7 && *in; i++, in++)
		exten[i] = *in;
	exten[i] = 0;
	return exten;
}

int UTIL_CountPlayers()
{
	int	num = 0;

	for( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pEnt = UTIL_PlayerByIndex( i );

		//TODO: this can be wrong if the client has disconnected. - Solokiller
		if( pEnt )
		{
			num = num + 1;
		}
	}

	return num;
}

void UTIL_AddToAssistList(CBaseEntity* pEnt)
{
	ALERT(at_console, "Add %s \"%s\" to AssistList\n", STRING(pEnt->pev->classname), STRING(pEnt->pev->targetname));

	if (pEnt->m_pAssistLink)
	{
		ALERT(at_console, "Ignored AddToAssistList for %s \"%s\"\n", STRING(pEnt->pev->classname), STRING(pEnt->pev->targetname));
		return;
	}

	if (!CWorld::GetInstance())
	{
		ALERT(at_console, "AddToAssistList %s \"%s\" has no AssistList!\n", STRING(pEnt->pev->classname), STRING(pEnt->pev->targetname));
		return;
	}

	CBaseEntity* pListMember = CWorld::GetInstance();

	// find the last entry in the list...
	while (pListMember->m_pAssistLink != nullptr)
		pListMember = pListMember->m_pAssistLink;

	if (pListMember == pEnt)
	{
		ALERT(at_console, "(end)Ignored AddToAssistList for %s \"%s\"\n", STRING(pEnt->pev->classname), STRING(pEnt->pev->targetname));
		return;
	}

	pListMember->m_pAssistLink = pEnt; // it's not in the list already, so add pEnt to the list.
}

void UTIL_MarkForAssist(CBaseEntity* pEnt, bool correctSpeed)
{
	pEnt->GetDesiredFlags() |= LF_DOASSIST;

	if (correctSpeed)
		pEnt->GetDesiredFlags() |= LF_CORRECTSPEED;
	else
		pEnt->GetDesiredFlags() &= ~LF_CORRECTSPEED;

	UTIL_AddToAssistList(pEnt);
}

void UTIL_SetMoveWithVelocity(CBaseEntity* pEnt, const Vector vecSet, int loopbreaker)
{
	if (loopbreaker <= 0)
	{
		ALERT(at_error, "Infinite child list for MoveWith!");
		return;
	}

	if (!pEnt->m_pMoveWith)
	{
		ALERT(at_error, "SetMoveWithVelocity: no MoveWith entity!?\n");
		return;
	}

	Vector vecNew = (pEnt->pev->velocity - pEnt->m_pMoveWith->pev->velocity) + vecSet;

	if (pEnt->m_pChildMoveWith)
	{
		CBaseEntity* pMoving = pEnt->m_pChildMoveWith;
		int sloopbreaker = MAX_MOVEWITH_DEPTH;
		while (pMoving)
		{
			UTIL_SetMoveWithVelocity(pMoving, vecNew, loopbreaker - 1);
			pMoving = pMoving->m_pSiblingMoveWith;
			sloopbreaker--;
			if (sloopbreaker <= 0)
			{
				ALERT(at_error, "SetMoveWithVelocity: Infinite sibling list for MoveWith!");
				break;
			}
		}
	}

	pEnt->pev->velocity = vecNew;
}

void UTIL_SetMoveWithAvelocity(CBaseEntity* pEnt, const Vector vecSet, int loopbreaker)
{
	if (loopbreaker <= 0)
	{
		ALERT(at_error, "Infinite child list for MoveWith!");
		return;
	}

	if (!pEnt->m_pMoveWith)
	{
		ALERT(at_error, "SetMoveWithAvelocity: no MoveWith entity!?\n");
		return;
	}

	Vector vecNew = (pEnt->pev->avelocity - pEnt->m_pMoveWith->pev->avelocity) + vecSet;

	//	ALERT(at_console, "Setting Child AVelocity %f %f %f, was %f %f %f mw %f %f %f\n", vecNew.x, vecNew.y, vecNew.z, pEnt->pev->avelocity.x, pEnt->pev->avelocity.y, pEnt->pev->avelocity.z, pEnt->m_pMoveWith->pev->avelocity.x, pEnt->m_pMoveWith->pev->avelocity.y, pEnt->m_pMoveWith->pev->avelocity.z);

	if (pEnt->m_pChildMoveWith)
	{
		CBaseEntity* pMoving = pEnt->m_pChildMoveWith;
		int sloopbreaker = MAX_MOVEWITH_DEPTH; // to prevent the game hanging...
		while (pMoving)
		{
			UTIL_SetMoveWithAvelocity(pMoving, vecNew, loopbreaker - 1);
			pMoving = pMoving->m_pSiblingMoveWith;
			sloopbreaker--;
			if (sloopbreaker <= 0)
			{
				ALERT(at_error, "SetMoveWithVelocity: Infinite sibling list for MoveWith!");
				break;
			}
		}
	}

	pEnt->pev->avelocity = vecNew;
}

void UTIL_AssignOrigin(CBaseEntity* pEntity, const Vector vecOrigin)
{
	UTIL_AssignOrigin(pEntity, vecOrigin, true);
}

void UTIL_AssignOrigin(CBaseEntity* pEntity, const Vector vecOrigin, bool bInitiator)
{
	Vector vecDiff = vecOrigin - pEntity->GetAbsOrigin();

	UTIL_SetOrigin(pEntity, vecOrigin);

	if (bInitiator && pEntity->m_pMoveWith)
	{
		pEntity->m_vecOffsetOrigin = pEntity->GetAbsOrigin() - pEntity->m_pMoveWith->GetAbsOrigin();
	}
	
	if (pEntity->m_pChildMoveWith) // now I've moved pEntity, does anything else have to move with it?
	{
		CBaseEntity* pChild = pEntity->m_pChildMoveWith;

		while (pChild)
		{
			if (pChild->pev->movetype != MOVETYPE_PUSH || pChild->pev->velocity == pEntity->pev->velocity) // if the child isn't moving under its own power
			{
				UTIL_AssignOrigin(pChild, vecOrigin + pChild->m_vecOffsetOrigin, FALSE);
			}
			else
			{
				Vector vecTemp = vecDiff + pChild->GetAbsOrigin();
				UTIL_AssignOrigin(pChild, vecTemp, FALSE);
			}
			pChild = pChild->m_pSiblingMoveWith;
		}
	}
}

void HandlePostAssist(CBaseEntity* pEnt)
{
	if (pEnt->m_iLFlags & LF_POSTASSISTVEL)
	{
		//		ALERT(at_console, "RestoreVel %s: orign %f %f %f, velocity was %f %f %f, back to %f %f %f\n",
		//			STRING(pEnt->pev->targetname),
		//			pEnt->GetAbsOrigin().x, pEnt->GetAbsOrigin().y, pEnt->GetAbsOrigin().z,
		//			pEnt->pev->velocity.x, pEnt->pev->velocity.y, pEnt->pev->velocity.z,
		//			pEnt->m_vecPostAssistVel.x, pEnt->m_vecPostAssistVel.y, pEnt->m_vecPostAssistVel.z
		//		);
		pEnt->pev->velocity = pEnt->m_vecPostAssistVel;
		pEnt->m_vecPostAssistVel = g_vecZero;
		pEnt->m_iLFlags &= ~LF_POSTASSISTVEL;
	}
	if (pEnt->m_iLFlags & LF_POSTASSISTAVEL)
	{
		//		ALERT(at_console, "RestoreVel %s: orign %f %f %f, velocity was %f %f %f, back to %f %f %f\n",
		//			STRING(pEnt->pev->targetname),
		//			pEnt->GetAbsOrigin().x, pEnt->GetAbsOrigin().y, pEnt->GetAbsOrigin().z,
		//			pEnt->pev->velocity.x, pEnt->pev->velocity.y, pEnt->pev->velocity.z,
		//			pEnt->m_vecPostAssistVel.x, pEnt->m_vecPostAssistVel.y, pEnt->m_vecPostAssistVel.z
		//		);
		pEnt->pev->avelocity = pEnt->m_vecPostAssistAVel;
		pEnt->m_vecPostAssistAVel = g_vecZero;
		pEnt->m_iLFlags &= ~LF_POSTASSISTAVEL;
	}
	
	CBaseEntity* pChild;
	for (pChild = pEnt->m_pChildMoveWith; pChild != nullptr; pChild = pChild->m_pSiblingMoveWith)
		HandlePostAssist(pChild);
}

int ApplyDesiredSettings(CBaseEntity* pListMember)
{
	if (pListMember->m_iLFlags & LF_DODESIRED)
	{
		pListMember->m_iLFlags &= ~LF_DODESIRED;
	}
	else
	{
		// don't need to apply any desired settings for this entity.
		return 0;
	}
	//	ALERT(at_console, "ApplyDesiredSettings for %s \"%s\", pevnt %f, ltime %f, mfnt %f, mpevnt %f, %f\n", STRING(pListMember->pev->classname), STRING(pListMember->pev->targetname), pListMember->pev->nextthink, pListMember->pev->ltime, pListMember->m_fNextThink, pListMember->m_fPevNextThink, pListMember->GetAbsOrigin().x);

	if (pListMember->m_iLFlags & LF_DESIRED_ACTION)
	{
		pListMember->m_iLFlags &= ~LF_DESIRED_ACTION;
		pListMember->DesiredAction();
		if (NeedUpdate(pListMember))
			SetBits(pListMember->m_pChildMoveWith->GetDesiredFlags(), LF_MERGEPOS);
	}

	if (pListMember->m_iLFlags & LF_DESIRED_INFO)
	{
		pListMember->m_iLFlags &= ~LF_DESIRED_INFO;
		ALERT(at_console, "DesiredInfo: pos %f %f %f, vel %f %f %f. Child pos %f %f %f, vel %f %f %f\n\n", pListMember->GetAbsOrigin().x, pListMember->GetAbsOrigin().y, pListMember->GetAbsOrigin().z, pListMember->pev->velocity.x, pListMember->pev->velocity.y, pListMember->pev->velocity.z, pListMember->m_pChildMoveWith->GetAbsOrigin().x, pListMember->m_pChildMoveWith->GetAbsOrigin().y, pListMember->m_pChildMoveWith->GetAbsOrigin().z, pListMember->m_pChildMoveWith->pev->velocity.x, pListMember->m_pChildMoveWith->pev->velocity.y, pListMember->m_pChildMoveWith->pev->velocity.z);
	}

	if (pListMember->m_iLFlags & LF_DESIRED_POSTASSIST)
	{
		pListMember->m_iLFlags &= ~LF_DESIRED_POSTASSIST;
		HandlePostAssist(pListMember);
	}

	if (pListMember->m_iLFlags & LF_DESIRED_THINK)
	{
		pListMember->m_iLFlags &= ~LF_DESIRED_THINK;
		//ALERT(at_console, "DesiredThink %s\n", STRING(pListMember->pev->targetname));
		pListMember->Think();
	}

	return 1;
}

void AssistChildren(CBaseEntity* pEnt, Vector vecAdjustVel, Vector vecAdjustAVel)
{
	CBaseEntity* pChild;
	for (pChild = pEnt->m_pChildMoveWith; pChild != NULL; pChild = pChild->m_pSiblingMoveWith)
	{
		if (!(pChild->m_iLFlags & LF_POSTASSISTVEL))
		{
			pChild->m_vecPostAssistVel = pChild->pev->velocity;
			pChild->m_iLFlags |= LF_POSTASSISTVEL;
		}
		if (!(pChild->m_iLFlags & LF_POSTASSISTAVEL))
		{
			pChild->m_vecPostAssistAVel = pChild->pev->avelocity;
			pChild->m_iLFlags |= LF_POSTASSISTAVEL;
		}
		pChild->pev->velocity = pChild->pev->velocity - vecAdjustVel;// (pChild->pev->velocity - pEnt->m_vecPostAssistVel) + pEnt->m_vecPostAssistVel*fFraction;
		pChild->pev->avelocity = pChild->pev->avelocity - vecAdjustAVel;// (pChild->pev->avelocity - pEnt->m_vecPostAssistAVel) + pEnt->m_vecPostAssistAVel*fFraction;

		//ALERT(at_console, "AssistChild %s: origin %f %f %f, old vel %f %f %f. fraction %f, new vel %f %f %f, dest %f %f %f\n", STRING(pChild->pev->targetname), pChild->GetAbsOrigin().x, pChild->GetAbsOrigin().y, pChild->GetAbsOrigin().z, pChild->m_vecPostAssistVel.x, pChild->m_vecPostAssistVel.y, pChild->m_vecPostAssistVel.z, fFraction, pChild->pev->velocity.x, pChild->pev->velocity.y, pChild->pev->velocity.z, pChild->GetAbsOrigin().x + pChild->pev->velocity.x*gpGlobals->frametime, pChild->GetAbsOrigin().y + pChild->pev->velocity.y*gpGlobals->frametime, pChild->GetAbsOrigin().z + pChild->pev->velocity.z*gpGlobals->frametime );
		//ALERT(at_console, "AssistChild %s: origin %f %f %f. velocity was %f %f %f, now %f %f %f\n", STRING(pChild->pev->targetname), pChild->GetAbsOrigin().x, pChild->GetAbsOrigin().y, pChild->GetAbsOrigin().z, pChild->m_vecPostAssistVel.x, pChild->m_vecPostAssistVel.y, pChild->m_vecPostAssistVel.z, pChild->pev->velocity.x, pChild->pev->velocity.y, pChild->pev->velocity.z);

		AssistChildren(pChild, vecAdjustVel, vecAdjustAVel);
	}
}

void UTIL_DesiredAction(CBaseEntity* pEnt)
{
	pEnt->m_iLFlags |= LF_DESIRED_ACTION;
	UTIL_MarkForDesired(pEnt);
}

void UTIL_DesiredThink(CBaseEntity* pEnt)
{
	//	ALERT(at_console, "Setting DesiredThink %s\n", STRING(pEnt->pev->targetname));
	pEnt->m_iLFlags |= LF_DESIRED_THINK;
	pEnt->DontThink();
	UTIL_MarkForDesired(pEnt);
}

void UTIL_DesiredInfo(CBaseEntity* pEnt)
{
	pEnt->m_iLFlags |= LF_DESIRED_INFO;
	UTIL_MarkForDesired(pEnt);
}

void UTIL_DesiredPostAssist(CBaseEntity* pEnt)
{
	pEnt->m_iLFlags |= LF_DESIRED_POSTASSIST;
	UTIL_MarkForDesired(pEnt);
}

void UTIL_MarkForDesired(CBaseEntity* pEnt)
{
	pEnt->m_iLFlags |= LF_DODESIRED;

	if (g_doingDesired)
	{
		//ALERT(at_console, "doingDesired is true, start immediately\n");
		ApplyDesiredSettings(pEnt);
		return;
	}

	UTIL_AddToAssistList(pEnt);
}

void UTIL_SetVelocity(CBaseEntity* pEnt, const Vector vecSet)
{
	Vector vecNew;
	if (pEnt->m_pMoveWith)
		vecNew = vecSet + pEnt->m_pMoveWith->pev->velocity;
	else
		vecNew = vecSet;

	//	ALERT(at_console,"SetV: %s is sent (%f,%f,%f) - goes from (%f,%f,%f) to (%f,%f,%f)\n",
	//	    STRING(pEnt->pev->targetname), vecSet.x, vecSet.y, vecSet.z,
	//		pEnt->pev->velocity.x, pEnt->pev->velocity.y, pEnt->pev->velocity.z,
	//		vecNew.x, vecNew.y, vecNew.z
	//	);

	if (pEnt->m_pChildMoveWith)
	{
		CBaseEntity* pMoving = pEnt->m_pChildMoveWith;
		int sloopbreaker = MAX_MOVEWITH_DEPTH; // LRC - to save us from infinite loops
		while (pMoving)
		{
			UTIL_SetMoveWithVelocity(pMoving, vecNew, MAX_MOVEWITH_DEPTH);
			if (vecSet != g_vecZero)SetBits(pMoving->m_iLFlags, LF_PARENTMOVE);
			else ClearBits(pMoving->m_iLFlags, LF_PARENTMOVE);
			pMoving = pMoving->m_pSiblingMoveWith;
			sloopbreaker--;
			if (sloopbreaker <= 0)
			{
				ALERT(at_error, "SetVelocity: Infinite sibling list for MoveWith!\n");
				break;
			}
		}
	}

	pEnt->pev->velocity = vecNew;
}

void UTIL_SetAvelocity(CBaseEntity* pEnt, const Vector vecSet)
{
	Vector vecNew;
	if (pEnt->m_pMoveWith)
		vecNew = vecSet + pEnt->m_pMoveWith->pev->avelocity;
	else
		vecNew = vecSet;

	//	ALERT(at_console, "Setting AVelocity %f %f %f\n", vecNew.x, vecNew.y, vecNew.z);

	if (pEnt->m_pChildMoveWith)
	{
		CBaseEntity* pMoving = pEnt->m_pChildMoveWith;
		int sloopbreaker = MAX_MOVEWITH_DEPTH; // LRC - to save us from infinite loops
		while (pMoving)
		{
			UTIL_SetMoveWithAvelocity(pMoving, vecNew, MAX_MOVEWITH_DEPTH);
			UTIL_MergePos(pMoving); //force to update
			if (vecSet != g_vecZero)SetBits(pMoving->m_iLFlags, LF_PARENTMOVE);
			else ClearBits(pMoving->m_iLFlags, LF_PARENTMOVE);
			pMoving = pMoving->m_pSiblingMoveWith;
			sloopbreaker--;
			if (sloopbreaker <= 0)
			{
				ALERT(at_error, "SetAvelocity: Infinite sibling list for MoveWith!\n");
				break;
			}
		}
	}
	pEnt->pev->avelocity = vecNew;
}

void UTIL_SetAngles(CBaseEntity* pEntity, const Vector vecAngles)
{
	UTIL_SetAngles(pEntity, vecAngles, TRUE);
}

void UTIL_SetAngles(CBaseEntity* pEntity, const Vector vecAngles, BOOL bInitiator)
{
	Vector vecDiff = vecAngles - pEntity->pev->angles;
	if (vecDiff.Length() > 0.01 && CVAR_GET_FLOAT("sohl_mwdebug"))
		ALERT(at_console, "SetAngles %s %s: (%f %f %f) goes to (%f %f %f)\n", STRING(pEntity->pev->classname), STRING(pEntity->pev->targetname), pEntity->pev->angles.x, pEntity->pev->angles.y, pEntity->pev->angles.z, vecAngles.x, vecAngles.y, vecAngles.z);

	//	UTIL_SetDesiredAngles(pEntity, vecAngles);
	pEntity->pev->angles = vecAngles;

	if (bInitiator && pEntity->m_pMoveWith)
	{
		pEntity->m_vecRotWithOffset = vecAngles - pEntity->m_pMoveWith->pev->angles;
	}
	if (pEntity->m_pChildMoveWith) // now I've moved pEntity, does anything else have to move with it?
	{
		CBaseEntity* pChild = pEntity->m_pChildMoveWith;
		Vector vecTemp;
		while (pChild)
		{
			if (pChild->pev->avelocity == pEntity->pev->avelocity) // if the child isn't turning under its own power
			{
				UTIL_SetAngles(pChild, vecAngles + pChild->m_vecRotWithOffset, FALSE);
			}
			else
			{
				vecTemp = vecDiff + pChild->pev->angles;
				UTIL_SetAngles(pChild, vecTemp, FALSE);
			}
			//ALERT(at_console,"  child origin becomes (%f %f %f)\n",pChild->GetAbsOrigin().x,pChild->GetAbsOrigin().y,pChild->GetAbsOrigin().z);
			//ALERT(at_console,"ent %p has sibling %p\n",pChild,pChild->m_pSiblingMoveWith);
			pChild = pChild->m_pSiblingMoveWith;
		}
	}
}

void UTIL_MergePos(CBaseEntity* pEnt, int loopbreaker)
{
	if (loopbreaker <= 0  || !pEnt->m_pMoveWith)
		return;

	Vector forward, right, up, vecOrg;
	UTIL_MakeVectorsPrivate(pEnt->m_pMoveWith->GetAbsAngles(), &forward, &right, &up);

	if (pEnt->m_pMoveWith->pev->flags & FL_MONSTER)
		vecOrg = pEnt->m_vecPostAssistOrg = pEnt->m_pMoveWith->GetAbsOrigin() + (forward * pEnt->m_vecOffsetOrigin.x) + (right * pEnt->m_vecOffsetOrigin.y) + (up * pEnt->m_vecOffsetOrigin.z);
	else	
		vecOrg = pEnt->m_vecPostAssistOrg = pEnt->m_pMoveWith->GetAbsOrigin() + (forward * pEnt->m_vecOffsetOrigin.x) + (-right * pEnt->m_vecOffsetOrigin.y) + (up * pEnt->m_vecOffsetOrigin.z);

	Vector vecAngles = pEnt->m_vecPostAssistAng = pEnt->m_pMoveWith->pev->angles + pEnt->m_vecOffsetAngles;

	if (pEnt->m_pChildMoveWith)
	{
		CBaseEntity* pMoving = pEnt->m_pChildMoveWith;
		int sloopbreaker = MAX_MOVEWITH_DEPTH;
		while (pMoving)
		{
			UTIL_MergePos(pMoving, loopbreaker - 1);
			pMoving = pMoving->m_pSiblingMoveWith;
			sloopbreaker--;
			if (sloopbreaker <= 0)break;
		}
	}

	if (pEnt->m_iLFlags & LF_MERGEPOS)
	{
		UTIL_AssignOrigin(pEnt, vecOrg);
		UTIL_SetAngles(pEnt, vecAngles);
		ClearBits(pEnt->m_iLFlags, LF_MERGEPOS);
	}
	
	if (pEnt->m_iLFlags & LF_POSTORG)
	{
		pEnt->SetAbsOrigin(vecOrg);
		pEnt->pev->angles = vecAngles;
		SetBits(pEnt->m_iLFlags, LF_DODESIRED); //refresh position every frame
		if (!pEnt->m_pAssistLink) 
			UTIL_AddToAssistList(pEnt);
	}
}

// called every frame, by StartFrame
void CheckAssistList()
{
	if (!CWorld::GetInstance())
	{
		ALERT(at_error, "CheckAssistList has no AssistList!\n");
		return;
	}

	CBaseEntity* pListMember = CWorld::GetInstance();
	while (pListMember->m_pAssistLink) // handle the remaining entries in the list
	{
		TryAssistEntity(pListMember->m_pAssistLink);
		if (!(pListMember->m_pAssistLink->m_iLFlags & LF_ASSISTLIST))
		{
			CBaseEntity* pTemp = pListMember->m_pAssistLink;
			pListMember->m_pAssistLink = pListMember->m_pAssistLink->m_pAssistLink;
			pTemp->m_pAssistLink = nullptr;
		}
		else pListMember = pListMember->m_pAssistLink;
	}
}

// called every frame, by PostThink
void CheckDesiredList()
{
	int loopbreaker = 1024; //max edicts

	if (g_doingDesired) 
		ALERT(at_console, "CheckDesiredList: doingDesired is already set!?\n");
	
	g_doingDesired = true;

	if (!CWorld::GetInstance())
	{
		ALERT(at_console, "CheckDesiredList has no AssistList!\n");
		return;
	}

	CBaseEntity* pListMember = CWorld::GetInstance()->m_pAssistLink;
	while (pListMember)
	{
		// cache this, in case ApplyDesiredSettings does a SUB_Remove.
		CBaseEntity* pNext = pListMember->m_pAssistLink;
		ApplyDesiredSettings(pListMember);
		pListMember = pNext;
		loopbreaker--;
		if (loopbreaker <= 0)
		{
			ALERT(at_error, "Infinite(?) loop in DesiredList!");
			break;
		}
	}

	g_doingDesired = false;
}

int TryAssistEntity(CBaseEntity* pEnt)
{
	if (!gpGlobals->frametime)
	{
		//ALERT(at_console, "frametime 0, don't assist\n");
		return 0;
	}
//	ALERT(at_console, "AssistList: %s\n", STRING(pEnt->pev->classname));

	if (pEnt->m_fNextThink <= 0)
	{
		//ALERT(at_console, "Cancelling assist for %s, %f\n", STRING(pEnt->pev->targetname), pEnt->pev->origin.x);
		pEnt->m_iLFlags &= ~LF_DOASSIST;
		return 0; // the think has been cancelled. Oh well...
	}

	// a fraction of the current velocity. (the part of it that the engine should see.)
	float fFraction = 0;

	// is this the frame when the entity will stop?
	if (pEnt->pev->movetype == MOVETYPE_PUSH)
	{
		if (pEnt->m_fNextThink <= pEnt->pev->ltime + gpGlobals->frametime)
			fFraction = (pEnt->m_fNextThink - pEnt->pev->ltime) / gpGlobals->frametime;
	}
	else if (pEnt->m_fNextThink <= gpGlobals->time + gpGlobals->frametime)
	{
		fFraction = (pEnt->m_fNextThink - gpGlobals->time) / gpGlobals->frametime;
		//ALERT(at_console, "Setting fFraction\n");
	}

	if (fFraction)
	{
		//ALERT(at_console, "Assisting %s \"%s\", %f <= %f + %f\n", STRING(pEnt->pev->classname), STRING(pEnt->pev->targetname), pEnt->m_fNextThink, pEnt->pev->ltime, gpGlobals->frametime);

		if (pEnt->m_iLFlags & LF_CORRECTSPEED)
		{
			if (!(pEnt->m_iLFlags & LF_POSTASSISTVEL))
			{
				pEnt->m_vecPostAssistVel = pEnt->GetAbsVelocity();
				pEnt->m_iLFlags |= LF_POSTASSISTVEL;
			}

			if (!(pEnt->m_iLFlags & LF_POSTASSISTAVEL))
			{
				pEnt->m_vecPostAssistAVel = pEnt->GetAngularVelocity();
				pEnt->m_iLFlags |= LF_POSTASSISTAVEL;
			}

			Vector vecVelTemp = pEnt->GetAbsVelocity();
			Vector vecAVelTemp = pEnt->GetAngularVelocity();

			if (pEnt->m_pMoveWith)
			{
				pEnt->pev->velocity = (pEnt->GetAbsVelocity() - pEnt->m_pMoveWith->GetAbsVelocity()) * fFraction + pEnt->m_pMoveWith->GetAbsVelocity();
				pEnt->pev->avelocity = (pEnt->GetAngularVelocity() - pEnt->m_pMoveWith->GetAngularVelocity()) * fFraction + pEnt->m_pMoveWith->GetAngularVelocity();
			}
			else
			{
				pEnt->pev->velocity = pEnt->pev->velocity * fFraction;
				pEnt->pev->avelocity = pEnt->GetAngularVelocity() * fFraction;
			}

			//ALERT(at_console, "Assist %s: origin %f %f %f, old vel %f %f %f. fraction %f, new vel %f %f %f, dest %f %f %f\n",
			//STRING(pEnt->pev->targetname), pEnt->pev->origin.x, pEnt->pev->origin.y, pEnt->pev->origin.z, pEnt->m_vecPostAssistVel.x,
			//pEnt->m_vecPostAssistVel.y, pEnt->m_vecPostAssistVel.z, fFraction, pEnt->pev->velocity.x, pEnt->pev->velocity.y, pEnt->pev->velocity.z,
			//pEnt->pev->origin.x + pEnt->pev->velocity.x*gpGlobals->frametime, pEnt->pev->origin.y + pEnt->pev->velocity.y*gpGlobals->frametime, pEnt->pev->origin.z + pEnt->pev->velocity.z*gpGlobals->frametime);

			AssistChildren(pEnt, vecVelTemp - pEnt->GetAbsVelocity(), vecAVelTemp - pEnt->GetAngularVelocity());
			UTIL_DesiredPostAssist(pEnt);
		}

		UTIL_DesiredThink(pEnt);
		//		ALERT(at_console, "Assist sets DesiredThink for %s\n", STRING(pEnt->pev->classname));

		pEnt->m_iLFlags &= ~LF_DOASSIST;
	}
	
	return 1;
}

bool NeedUpdate(CBaseEntity* pEnt)
{
	if (pEnt->m_pChildMoveWith && pEnt->m_pChildMoveWith->m_vecOffsetOrigin == g_vecZero)//potentially loser
	{
		if (pEnt->GetAbsOrigin() != pEnt->m_pChildMoveWith->GetAbsOrigin())
		{
			ALERT(at_console, "Warning %s lose synch with child\n", pEnt->GetClassname());
			return true;
		}
	}
	
	return false;
}

void UTIL_AddToAliasList(CBaseAlias* pAlias)
{
	CWorld * g_pWorld = CWorld::GetInstance();
	if (!g_pWorld)
	{
		ALERT(at_console, "AddToAliasList has no AliasList!\n");
		return;
	}

	pAlias->m_iLFlags |= LF_ALIASLIST;

	//	ALERT(at_console, "Adding %s \"%s\" to alias list\n", STRING(pAlias->pev->classname), STRING(pAlias->pev->targetname));
	if (g_pWorld->m_pFirstAlias == nullptr)
	{
		g_pWorld->m_pFirstAlias = pAlias;
		pAlias->m_pNextAlias = nullptr;
	}
	else if (g_pWorld->m_pFirstAlias == pAlias)
	{
		// already in the list
		return;
	}
	else
	{
		CBaseAlias* pCurrent = g_pWorld->m_pFirstAlias;
		while (pCurrent->m_pNextAlias != nullptr)
		{
			if (pCurrent->m_pNextAlias == pAlias)
			{
				// already in the list
				return;
			}

			pCurrent = pCurrent->m_pNextAlias;
		}

		pCurrent->m_pNextAlias = pAlias;
		pAlias->m_pNextAlias = nullptr;
	}
}

#define MAX_ALIASNAME_LEN 80

// Returns the first entity which szName refers to and which is after pStartEntity.
CBaseEntity* UTIL_FollowReference(CBaseEntity* pStartEntity, const char* szName)
{
	char szRoot[MAX_ALIASNAME_LEN + 1]; // allow room for null-terminator
	CBaseEntity* pResult;

	if (!szName || szName[0] == 0) 
		return nullptr;

	// reference through an info_group?
	for (int i = 0; szName[i]; i++)
	{
		if (szName[i] == '.')
		{
			// yes, it looks like a reference through an info_group...
			// FIXME: we should probably check that i < MAX_ALIASNAME_LEN.
			strncpy(szRoot, szName, i);
			szRoot[i] = 0;
			char* szMember = (char*)&szName[i + 1];
			//ALERT(at_console,"Following reference- group %s with member %s\n",szRoot,szMember);
			pResult = UTIL_FollowGroupReference(pStartEntity, szRoot, szMember);
			//if (pResult)
			//ALERT(at_console,"\"%s\".\"%s\" = %s\n",szRoot,szMember,STRING(pResult->pev->targetname));
			return pResult;
		}
	}
	// reference through an info_alias?
	if (szName[0] == '*')
	{
		if (FStrEq(szName, "*player"))
		{
			CBaseEntity* pPlayer = UTIL_FindEntityByClassname(nullptr, "player");
			if (pPlayer && (pStartEntity == nullptr || pPlayer->eoffset() > pStartEntity->eoffset()))
				return pPlayer;

			return nullptr;
		}
		//ALERT(at_console,"Following alias %s\n",szName+1);
		pResult = UTIL_FollowAliasReference(pStartEntity, szName + 1);
		//if (pResult)
		//ALERT(at_console,"alias \"%s\" = %s\n",szName+1,STRING(pResult->pev->targetname));
		return pResult;
	}
	// not a reference
//	ALERT(at_console,"%s is not a reference\n",szName);
	return nullptr;
}

// for every info_group which has the given groupname, find the earliest entity which is referred to by its member
// with the given membername and which is later than pStartEntity.
CBaseEntity* UTIL_FollowGroupReference(CBaseEntity* pStartEntity, char* szGroupName, char* szMemberName)
{
	CBaseEntity* pBestEntity = nullptr; // the entity we're currently planning to return.
	int iBestOffset = -1; // the offset of that entity.
	CBaseEntity* pTempEntity;
	char szBuf[MAX_ALIASNAME_LEN];
	char* szThisMember = szMemberName;
	char* szTail = nullptr;

	// find the first '.' in the membername and if there is one, split the string at that point.
	for (int i = 0; szMemberName[i]; i++)
	{
		if (szMemberName[i] == '.')
		{
			// recursive member-reference
			// FIXME: we should probably check that i < MAX_ALIASNAME_LEN.
			strncpy(szBuf, szMemberName, i);
			szBuf[i] = 0;
			szTail = &(szMemberName[i + 1]);
			szThisMember = szBuf;
			break;
		}
	}

	CBaseEntity* pEntity = UTIL_FindEntityByTargetname(nullptr, szGroupName);
	while (pEntity)
	{
		if (FStrEq(STRING(pEntity->pev->classname), "info_group"))
		{
			int iszMemberValue = ((CInfoGroup*)pEntity)->GetMember(szThisMember);
			//			ALERT(at_console,"survived getMember\n");
			//			return NULL;
			if (!FStringNull(iszMemberValue))
			{
				if (szTail) // do we have more references to follow?
					pTempEntity = UTIL_FollowGroupReference(pStartEntity, (char*)STRING(iszMemberValue), szTail);
				else
					pTempEntity = UTIL_FindEntityByTargetname(pStartEntity, STRING(iszMemberValue));

				if (pTempEntity)
				{
					int iTempOffset = OFFSET(pTempEntity->pev);
					if (iBestOffset == -1 || iTempOffset < iBestOffset)
					{
						iBestOffset = iTempOffset;
						pBestEntity = pTempEntity;
					}
				}
			}
		}
		pEntity = UTIL_FindEntityByTargetname(pEntity, szGroupName);
	}

	if (pBestEntity)
	{
		//		ALERT(at_console,"\"%s\".\"%s\" returns %s\n",szGroupName,szMemberName,STRING(pBestEntity->pev->targetname));
		return pBestEntity;
	}

	return nullptr;
}

// for every alias which has the given name, find the earliest entity which any of them refers to
// and which is later than pStartEntity.
CBaseEntity* UTIL_FollowAliasReference(CBaseEntity* pStartEntity, const char* szValue)
{
	CBaseEntity* pBestEntity = nullptr; // the entity we're currently planning to return.
	int iBestOffset = -1; // the offset of that entity.

	CBaseEntity* pEntity = UTIL_FindEntityByTargetname(nullptr, szValue);

	while (pEntity)
	{
		if (pEntity->IsAlias())
		{
			CBaseEntity* pTempEntity = ((CBaseAlias*)pEntity)->FollowAlias(pStartEntity);
			if (pTempEntity)
			{
				// We've found an entity; only use it if its offset is lower than the offset we've currently got.
				int iTempOffset = OFFSET(pTempEntity->pev);
				if (iBestOffset == -1 || iTempOffset < iBestOffset)
				{
					iBestOffset = iTempOffset;
					pBestEntity = pTempEntity;
				}
			}
		}
		pEntity = UTIL_FindEntityByTargetname(pEntity, szValue);
	}

	return pBestEntity;
}

void UTIL_MuzzleLight(Vector vecSrc, float flRadius, byte r, byte g, byte b, float flTime, float flDecay)
{
	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, vecSrc);
		WRITE_BYTE(TE_DLIGHT);
		WRITE_COORD(vecSrc.x);	// X
		WRITE_COORD(vecSrc.y);	// Y
		WRITE_COORD(vecSrc.z);	// Z
		WRITE_BYTE(flRadius * 0.1f);	// radius * 0.1
		WRITE_BYTE(r);		// r
		WRITE_BYTE(g);		// g
		WRITE_BYTE(b);		// b
		WRITE_BYTE(flTime * 10.0f);	// time * 10
		WRITE_BYTE(flDecay * 0.1f);	// decay * 0.1
	MESSAGE_END();
}

//=========================================================
// UTIL_StripToken - for redundant keynames
//=========================================================
void UTIL_StripToken(const char* pKey, char* pDest)
{
	int i = 0;

	while (pKey[i] && pKey[i] != '#')
	{
		pDest[i] = pKey[i];
		i++;
	}
	pDest[i] = 0;
}

//=========================================================
// UTIL_DotPoints - returns the dot product of a line from
// src to check and vecdir.
//=========================================================
float UTIL_DotPoints(const Vector& vecSrc, const Vector& vecCheck, const Vector& vecDir)
{
	Vector2D vec2LOS = (vecCheck - vecSrc).Make2D();
	vec2LOS = vec2LOS.Normalize();

	return DotProduct(vec2LOS, (vecDir.Make2D()));
}

//LRC - randomized vectors of the form "0 0 0 .. 1 0 0"
void UTIL_StringToRandomVector(float* pVector, const char* pString)
{
	char* pstr, * pfront, tempString[128];
	int	j;
	float pAltVec[3];

	strcpy(tempString, pString);
	pstr = pfront = tempString;

	for (j = 0; j < 3; j++)			// lifted from pr_edict.c
	{
		pVector[j] = atof(pfront);

		while (*pstr && *pstr != ' ') pstr++;
		if (!*pstr) break;
		pstr++;
		pfront = pstr;
	}
	if (j < 2)
	{
		/*
		ALERT( at_error, "Bad field in entity!! %s:%s == \"%s\"\n",
			pkvd->szClassName, pkvd->szKeyName, pkvd->szValue );
		*/
		for (j = j + 1; j < 3; j++)
			pVector[j] = 0;
	}
	else if (*pstr == '.')
	{
		pstr++;
		if (*pstr != '.') return;
		pstr++;
		if (*pstr != ' ') return;

		UTIL_StringToVector(pAltVec, pstr);

		pVector[0] = RANDOM_FLOAT(pVector[0], pAltVec[0]);
		pVector[1] = RANDOM_FLOAT(pVector[1], pAltVec[1]);
		pVector[2] = RANDOM_FLOAT(pVector[2], pAltVec[2]);
	}
}

//LRC - randomized vectors of the form "0 0 0 .. 1 0 0"
void UTIL_StringToVector(float* pVector, const char* pString)
{
	char* pstr, * pfront, tempString[128];
	int	j;
	float pAltVec[3];

	strcpy(tempString, pString);
	pstr = pfront = tempString;

	for (j = 0; j < 3; j++)			// lifted from pr_edict.c
	{
		pVector[j] = atof(pfront);

		while (*pstr && *pstr != ' ') pstr++;
		if (!*pstr) break;
		pstr++;
		pfront = pstr;
	}
	if (j < 2)
	{
		/*
		ALERT( at_error, "Bad field in entity!! %s:%s == \"%s\"\n",
			pkvd->szClassName, pkvd->szKeyName, pkvd->szValue );
		*/
		for (j = j + 1; j < 3; j++)
			pVector[j] = 0;
	}
	else if (*pstr == '.')
	{
		pstr++;
		if (*pstr != '.') return;
		pstr++;
		if (*pstr != ' ') return;

		UTIL_StringToVector(pAltVec, pstr);

		pVector[0] = RANDOM_FLOAT(pVector[0], pAltVec[0]);
		pVector[1] = RANDOM_FLOAT(pVector[1], pAltVec[1]);
		pVector[2] = RANDOM_FLOAT(pVector[2], pAltVec[2]);
	}
}

//LRC - as above, but returns the position of point 1 0 0 under the given rotation
Vector UTIL_AxisRotationToVec(const Vector& vecAxis, float flDegs)
{
	float rgflVecOut[3];
	float flRads = flDegs * (M_PI / 180.0);
	float c = cos(flRads);
	float s = sin(flRads);
	float v = vecAxis.x * (1 - c);

	//ugh, more maths. Thank goodness for internet geometry sites...
	rgflVecOut[0] = vecAxis.x * v + c;
	rgflVecOut[1] = vecAxis.y * v + vecAxis.z * s;
	rgflVecOut[2] = vecAxis.z * v - vecAxis.y * s;
	return Vector(rgflVecOut[0], rgflVecOut[1], rgflVecOut[2]);
}