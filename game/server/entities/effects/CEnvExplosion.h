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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CENVEXPLOSION_H
#define GAME_SERVER_ENTITIES_EFFECTS_CENVEXPLOSION_H

#define	SF_ENVEXPLOSION_NODAMAGE	( 1 << 0 ) // when set, ENV_EXPLOSION will not actually inflict damage
#define	SF_ENVEXPLOSION_REPEATABLE	( 1 << 1 ) // can this entity be refired?
#define SF_ENVEXPLOSION_NOFIREBALL	( 1 << 2 ) // don't draw the fireball
#define SF_ENVEXPLOSION_NOSMOKE		( 1 << 3 ) // don't draw the smoke
#define SF_ENVEXPLOSION_NODECAL		( 1 << 4 ) // don't make a scorch mark
#define SF_ENVEXPLOSION_NOSPARKS	( 1 << 5 ) // don't make a scorch mark

class CEnvExplosion : public CBaseMonster
{
public:
	DECLARE_CLASS( CEnvExplosion, CBaseMonster );
	DECLARE_DATADESC();

	void Spawn() override;
	void Smoke( void );
	void KeyValue( KeyValueData *pkvd ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	/**
	*	Sets the magnitude of the explosion.
	*/
	void SetMagnitude( int iMagnitude )
	{
		m_iMagnitude = iMagnitude;
	}

	int m_iMagnitude;// how large is the fireball? how much damage?
	int m_spriteScale; // what's the exact fireball sprite scale? 
};

extern DLL_GLOBAL	short	g_sModelIndexFireball;
extern DLL_GLOBAL	short	g_sModelIndexSmoke;

/**
*	Creates an explosion.
*	@param vecCenter Center point of the explosion.
*	@param vecAngles Explosion angles.
*	@param pOwner Optional. The owner of the explosion. Used to keep track of frags and perform damage filtering.
*	@param iMagnitude Magnitude of the explosion.
*	@param bDoDamage Whether to deal damage or not.
*	@param flDelay Optional. Delay until the explosion occurs.
*	@param flRandomRange Optional. Random offset to apply to the explosion's X and Y axes.
*/
void UTIL_CreateExplosion( Vector vecCenter, const Vector& vecAngles, CBaseEntity* pOwner, int iMagnitude, const bool bDoDamage, const float flDelay = 0, const float flRandomRange = 0 );

#endif //GAME_SERVER_ENTITIES_EFFECTS_CENVEXPLOSION_H
