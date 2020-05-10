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
#ifndef CLIENT_H
#define CLIENT_H

enum HostFlag
{
	HOSTFL_NONE			= 0,

	/**
	*	cl_lw is non-zero for this client.
	*/
	HOSTFL_WEAPONPRED	= 1 << 0
};

namespace SayType
{
enum SayType
{
	SAY			= 0,
	SAY_TEAM	= 1
};
}

qboolean ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ CCONNECT_REJECT_REASON_SIZE ] );
void ClientDisconnect( edict_t *pEntity );
void ClientKill( edict_t *pEntity );
void ClientPutInServer( edict_t *pEntity );
void ClientCommand( edict_t *pEntity );
void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer );
void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
void ServerDeactivate( void );
void StartFrame( void );
void PlayerPostThink( edict_t *pEntity );
void PlayerPreThink( edict_t *pEntity );
void ParmsNewLevel( void );
void ParmsChangeLevel( void );

void ClientPrecache( void );

const char *GetGameDescription();
void PlayerCustomization( edict_t *pEntity, customization_t *pCust );

void SpectatorConnect ( edict_t *pEntity );
void SpectatorDisconnect ( edict_t *pEntity );
void SpectatorThink ( edict_t *pEntity );

void Sys_Error( const char *error_string );

void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas );
void	UpdateClientData ( const edict_t* pClient, int sendweapons, clientdata_t* cd );
int AddToFullPack( entity_state_t *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet );
void CreateBaseline( int player, int eindex, entity_state_t* baseline, edict_t* entity, int playermodelindex,
					 const Vector player_mins[ Hull::COUNT ], const Vector player_maxs[ Hull::COUNT ] );
void RegisterEncoders();

int GetWeaponData( edict_t* pPlayer, weapon_data_t* pInfo );

void CmdStart( const edict_t *player, const usercmd_t *cmd, unsigned int random_seed );
void CmdEnd ( const edict_t *player );

int	ConnectionlessPacket( const netadr_t *net_from, const char *args, char *response_buffer, int *response_buffer_size );

int GetHullBounds( int hullnumber, Vector& mins, Vector& maxs );

void CreateInstancedBaselines ();

int	InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message );

int AllowLagCompensation();

#endif		// CLIENT_H
