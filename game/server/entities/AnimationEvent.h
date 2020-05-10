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
#ifndef GAME_SERVER_ENTITIES_NPCS_ANIMATIONEVENT_H
#define GAME_SERVER_ENTITIES_NPCS_ANIMATIONEVENT_H

/**
*	An animation event.
*	Called a monster event in the SDK, probably because it was originally added for monsters.
*/
struct AnimEvent_t
{
	int				event;
	const char*		options;
};

enum EventType
{
	EVENT_SPECIFIC	= 0,
	EVENT_SCRIPTED	= 1000,
	EVENT_SHARED	= 2000,

	/**
	*	All events larger than or equal to this are client side events. Not processed by the server.
	*/
	EVENT_CLIENT	= 5000,
};

#define MONSTER_EVENT_BODYDROP_LIGHT	2001
#define MONSTER_EVENT_BODYDROP_HEAVY	2002

#define MONSTER_EVENT_SWISHSOUND		2010

#endif //GAME_SERVER_ENTITIES_NPCS_ANIMATIONEVENT_H
