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
#ifndef GAME_SERVER_ENTITIES_PLATS_CFUNCTRACKCHANGE_H
#define GAME_SERVER_ENTITIES_PLATS_CFUNCTRACKCHANGE_H

#include "CFuncPlatRot.h"

class CPathTrack;
class CFuncTrackTrain;

#define SF_TRACK_ACTIVATETRAIN		0x00000001
#define SF_TRACK_RELINK				0x00000002
#define SF_TRACK_ROTMOVE			0x00000004
#define SF_TRACK_STARTBOTTOM		0x00000008
#define SF_TRACK_DONT_MOVE			0x00000010

enum TRAIN_CODE 
{
	TRAIN_SAFE,
	TRAIN_BLOCKING,
	TRAIN_FOLLOWING
};

//
// This entity is a rotating/moving platform that will carry a train to a new track.
// It must be larger in X-Y planar area than the train, since it must contain the
// train within these dimensions in order to operate when the train is near it.
//
class CFuncTrackChange : public CFuncPlatRot
{
public:
	DECLARE_CLASS( CFuncTrackChange, CFuncPlatRot );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;

	//	virtual void	Blocked( void ) override;
	virtual void	GoUp( void ) override;
	virtual void	GoDown( void ) override;

	void			KeyValue( KeyValueData* pkvd ) override;
	void			Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void			Find( void );
	TRAIN_CODE		EvaluateTrain( CPathTrack *pcurrent );
	void			UpdateTrain( Vector &dest );
	virtual void	HitBottom( void ) override;
	virtual void	HitTop( void ) override;
	void			Touch( CBaseEntity *pOther ) override;
	virtual void	UpdateAutoTargets( int toggleState );
	virtual	bool	IsTogglePlat() const override { return true; }

	void			DisableUse( void ) { m_use = 0; }
	void			EnableUse( void ) { m_use = 1; }
	int				UseEnabled( void ) { return m_use; }

	virtual void	OverrideReset( void ) override;


	CPathTrack		*m_trackTop;
	CPathTrack		*m_trackBottom;

	CFuncTrackTrain	*m_train;

	int				m_trackTopName;
	int				m_trackBottomName;
	int				m_trainName;
	TRAIN_CODE		m_code;
	int				m_targetState;
	int				m_use;
};

#endif //GAME_SERVER_ENTITIES_PLATS_CFUNCTRACKCHANGE_H