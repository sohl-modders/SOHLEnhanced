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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CGLOW_H
#define GAME_SERVER_ENTITIES_EFFECTS_CGLOW_H

class CGlow : public CPointEntity
{
public:
	DECLARE_CLASS( CGlow, CPointEntity );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Think( void ) override;
	void Animate( float frames );

	float		m_lastTime;
	float		m_maxFrame;
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CGLOW_H