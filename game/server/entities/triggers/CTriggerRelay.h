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
#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERDELAY_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERDELAY_H

#define SF_RELAY_FIREONCE		0x0001

class CTriggerRelay : public CBaseDelay
{
public:
	DECLARE_CLASS( CTriggerRelay, CBaseDelay );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData *pkvd ) override;
	void Spawn() override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	int ObjectCaps() const override { return CBaseDelay::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

private:
	USE_TYPE	triggerType;
};

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERDELAY_H