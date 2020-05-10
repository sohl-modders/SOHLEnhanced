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
#ifndef GAME_SERVER_ENTITIES_CYCLER_CCYCLERSPRITE_H
#define GAME_SERVER_ENTITIES_CYCLER_CCYCLERSPRITE_H

//TODO: not in the default fgd. Find a newer one or add it - Solokiller
class CCyclerSprite : public CBaseEntity
{
public:
	DECLARE_CLASS( CCyclerSprite, CBaseEntity );
	DECLARE_DATADESC();

	void Spawn() override;
	void Think() override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	virtual int	ObjectCaps() const override { return ( CBaseEntity::ObjectCaps() | FCAP_DONT_SAVE | FCAP_IMPULSE_USE ); }
	virtual void OnTakeDamage( const CTakeDamageInfo& info ) override;
	void	Animate( float frames );

	inline bool ShouldAnimate() const { return m_animate && m_maxFrame > 1.0; }
	int			m_animate;
	float		m_lastTime;
	float		m_maxFrame;
};

#endif //GAME_SERVER_ENTITIES_CYCLER_CCYCLERSPRITE_H