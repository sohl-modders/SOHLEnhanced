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
#ifndef GAME_SERVER_ENTITIES_TRIGGER_CTRIGGERPUSH_H
#define GAME_SERVER_ENTITIES_TRIGGER_CTRIGGERPUSH_H

#include "CBaseTrigger.h"

#define SF_TRIG_PUSH_ONCE		1
#define	SF_TRIGGER_PUSH_START_OFF	2//spawnflag that makes trigger_push spawn turned OFF

/*QUAKED trigger_push (.5 .5 .5) ? TRIG_PUSH_ONCE
Pushes the player
*/
class CTriggerPush : public CBaseTrigger
{
public:
	DECLARE_CLASS( CTriggerPush, CBaseTrigger );

	void Spawn( void ) override;
	void KeyValue( KeyValueData *pkvd ) override;
	void Touch( CBaseEntity *pOther ) override;
};

#endif //GAME_SERVER_ENTITIES_TRIGGER_CTRIGGERPUSH_H