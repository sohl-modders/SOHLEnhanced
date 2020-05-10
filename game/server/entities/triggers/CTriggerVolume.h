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
#ifndef GAME_SERVER_TRIGGERS_ENTITIES_CTRIGGERVOLUME_H
#define GAME_SERVER_TRIGGERS_ENTITIES_CTRIGGERVOLUME_H

// Define space that travels across a level transition
class CTriggerVolume : public CPointEntity	// Derive from point entity so this doesn't move across levels
{
public:
	DECLARE_CLASS( CTriggerVolume, CPointEntity );

	void		Spawn( void ) override;
};

#endif //GAME_SERVER_TRIGGERS_ENTITIES_CTRIGGERVOLUME_H