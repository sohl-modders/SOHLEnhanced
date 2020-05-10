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
#ifndef GAME_SERVER_ENTITIES_CACTANIMATING_H
#define GAME_SERVER_ENTITIES_CACTANIMATING_H

class CActAnimating : public CBaseAnimating
{
public:
	DECLARE_CLASS( CActAnimating, CBaseAnimating );
	DECLARE_DATADESC();

	void			SetActivity( Activity act );
	inline Activity	GetActivity() { return m_Activity; }

	virtual int	ObjectCaps() const override { return CBaseAnimating::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

private:
	Activity	m_Activity;
};

#endif //GAME_SERVER_ENTITIES_CACTANIMATING_H