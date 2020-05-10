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
#ifndef GAME_SERVER_ENTITIES_PLATS_CFUNCTRAINCONTROLS_H
#define GAME_SERVER_ENTITIES_PLATS_CFUNCTRAINCONTROLS_H

// This class defines the volume of space that the player must stand in to control the train
class CFuncTrainControls : public CBaseEntity
{
public:
	DECLARE_CLASS( CFuncTrainControls, CBaseEntity );
	DECLARE_DATADESC();

	virtual int	ObjectCaps() const override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	void Spawn() override;
	void Find();
};

#endif //GAME_SERVER_ENTITIES_PLATS_CFUNCTRAINCONTROLS_H