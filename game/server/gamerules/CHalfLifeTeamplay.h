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
#ifndef GAME_SERVER_GAMERULES_CHALFLIFETEAMPLAY_H
#define GAME_SERVER_GAMERULES_CHALFLIFETEAMPLAY_H

#include "CHalfLifeMultiplay.h"

#define MAX_TEAMNAME_LENGTH	16
#define MAX_TEAMS			32

#define TEAMPLAY_TEAMLISTLENGTH		MAX_TEAMS*MAX_TEAMNAME_LENGTH

/**
*	Rules for the basic half life teamplay competition.
*/
class CHalfLifeTeamplay : public CHalfLifeMultiplay
{
public:
	CHalfLifeTeamplay();

	virtual bool ClientCommand( CBasePlayer *pPlayer, const char *pcmd ) override;
	virtual void ClientUserInfoChanged( CBasePlayer *pPlayer, char *infobuffer ) override;
	virtual bool IsTeamplay() const override;
	virtual bool FPlayerCanTakeDamage( CBasePlayer *pPlayer, const CTakeDamageInfo& info ) override;
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget ) override;
	virtual const char *GetTeamID( CBaseEntity *pEntity ) override;
	virtual bool ShouldAutoAim( CBasePlayer *pPlayer, CBaseEntity* pTarget ) override;
	virtual int IPointsForKill( CBasePlayer *pAttacker, CBasePlayer *pKilled ) override;
	virtual void InitHUD( CBasePlayer *pl ) override;
	virtual void DeathNotice( CBasePlayer* pVictim, const CTakeDamageInfo& info ) override;
	virtual const char *GetGameDescription() const override { return "HL Teamplay"; }
	virtual void UpdateGameMode( CBasePlayer *pPlayer ) override;
	virtual void PlayerKilled( CBasePlayer* pVictim, const CTakeDamageInfo& info ) override;
	virtual void Think() override;
	virtual int GetTeamIndex( const char *pTeamName ) override;
	virtual const char *GetIndexedTeamName( int teamIndex ) override;
	virtual bool IsValidTeam( const char *pTeamName ) override;
	const char *SetDefaultPlayerTeam( CBasePlayer *pPlayer ) override;
	virtual void ChangePlayerTeam( CBasePlayer *pPlayer, const char *pTeamName, const bool bKill, const bool bGib ) override;

private:
	void RecountTeams( bool bResendInfo = false );
	const char *TeamWithFewestPlayers();

	bool m_DisableDeathMessages;
	bool m_DisableDeathPenalty;
	bool m_teamLimit;				// This means the server set only some teams as valid
	char m_szTeamList[TEAMPLAY_TEAMLISTLENGTH];
};

#endif //GAME_SERVER_GAMERULES_CHALFLIFETEAMPLAY_H
