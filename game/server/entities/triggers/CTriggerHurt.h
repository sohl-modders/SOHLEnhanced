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
#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERHURT_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERHURT_H

#include "CBaseTrigger.h"

#define SF_TRIGGER_HURT_TARGETONCE	1// Only fire hurt target once
#define	SF_TRIGGER_HURT_START_OFF	2//spawnflag that makes trigger_push spawn turned OFF
#define	SF_TRIGGER_HURT_NO_CLIENTS	8//spawnflag that makes trigger_push spawn turned OFF
#define SF_TRIGGER_HURT_CLIENTONLYFIRE	16// trigger hurt will only fire its target if it is hurting a client
#define SF_TRIGGER_HURT_CLIENTONLYTOUCH 32// only clients may touch this trigger.

class CTriggerHurt : public CBaseTrigger
{
public:
	DECLARE_CLASS( CTriggerHurt, CBaseTrigger );
	DECLARE_DATADESC();

	void Spawn( void ) override;

	void HurtTouch( CBaseEntity *pOther );

	void RadiationThink( void );
};

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERHURT_H