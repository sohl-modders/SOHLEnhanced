//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include <cstring>
#include <cstdio>

#include "hud.h"

#include "voice_banmgr.h"

#define BANMGR_FILEVERSION	1
const char* const g_pszBanMgrFilename = "voice_ban.dt";

unsigned char HashPlayerID( const char playerID[ PLAYERID_BUFFER_SIZE ] )
{
	unsigned char curHash = 0;

	for(size_t i = 0; i < PLAYERID_BUFFER_SIZE; ++i )
		curHash += (unsigned char)playerID[i];

	return curHash;
}

CVoiceBanMgr::CVoiceBanMgr()
{
	Clear();
}

CVoiceBanMgr::~CVoiceBanMgr()
{
	Term();
}

bool CVoiceBanMgr::Init( const char* const pszGameDir )
{
	Term();

	char filename[512];
	snprintf( filename, sizeof( filename ), "%s/%s", pszGameDir, g_pszBanMgrFilename );

	// Load in the squelch file.
	if( FILE* fp = fopen( filename, "rb" ) )
	{
		//Prevent this from changing without errors. This requires a file version change. - Solokiller
		static_assert( PLAYERID_BUFFER_SIZE == 16, "CVoiceBanMgr::Init: Player ID buffer size has changed!" );

		int version;
		fread(&version, 1, sizeof(version), fp);
		if(version == BANMGR_FILEVERSION)
		{
			fseek(fp, 0, SEEK_END);
			int nIDs = (ftell(fp) - sizeof(version)) / PLAYERID_BUFFER_SIZE;
			fseek(fp, sizeof(version), SEEK_SET);

			for(int i=0; i < nIDs; i++)
			{
				char playerID[ PLAYERID_BUFFER_SIZE ];
				fread(playerID, 1, PLAYERID_BUFFER_SIZE, fp);
				AddBannedPlayer(playerID);
			}			
		}

		fclose(fp);
	}

	return true;
}

void CVoiceBanMgr::Term()
{
	// Free all the player structures.
	for(size_t i = 0; i < MAX_BANNED_PLAYERS_HASH; ++i )
	{
		BannedPlayer *pListHead = &m_PlayerHash[ i ];
		BannedPlayer *pNext;
		for(BannedPlayer *pCur=pListHead->m_pNext; pCur != pListHead; pCur=pNext)
		{
			pNext = pCur->m_pNext;
			delete pCur;
		}
	}

	Clear();
}

void CVoiceBanMgr::SaveState( const char* const pszGameDir )
{
	// Save the file out.
	char filename[512];
	snprintf( filename, sizeof( filename ), "%s/%s", pszGameDir, g_pszBanMgrFilename );

	if( FILE* fp = fopen( filename, "wb" ) )
	{
		int version = BANMGR_FILEVERSION;
		fwrite(&version, 1, sizeof(version), fp);

		for( size_t i = 0; i < MAX_BANNED_PLAYERS_HASH; ++i )
		{
			BannedPlayer *pListHead = &m_PlayerHash[ i ];
			for(BannedPlayer *pCur=pListHead->m_pNext; pCur != pListHead; pCur=pCur->m_pNext)
			{
				fwrite(pCur->m_PlayerID, 1, PLAYERID_BUFFER_SIZE, fp);
			}
		}

		fclose(fp);
	}
}

bool CVoiceBanMgr::GetPlayerBan(char const playerID[ PLAYERID_BUFFER_SIZE ])
{
	return !!InternalFindPlayerSquelch(playerID);
}

void CVoiceBanMgr::SetPlayerBan(char const playerID[ PLAYERID_BUFFER_SIZE ], bool bSquelch)
{
	if(bSquelch)
	{
		// Is this guy already squelched?
		if(GetPlayerBan(playerID))
			return;
	
		AddBannedPlayer(playerID);
	}
	else
	{
		BannedPlayer *pPlayer = InternalFindPlayerSquelch(playerID);
		if(pPlayer)
		{
			pPlayer->m_pPrev->m_pNext = pPlayer->m_pNext;
			pPlayer->m_pNext->m_pPrev = pPlayer->m_pPrev;
			delete pPlayer;
		}
	}
}

void CVoiceBanMgr::ForEachBannedPlayer( ForEachCallback callback )
{
	for(size_t i = 0; i < MAX_BANNED_PLAYERS_HASH; ++i )
	{
		for(BannedPlayer *pCur=m_PlayerHash[i].m_pNext; pCur != &m_PlayerHash[i]; pCur=pCur->m_pNext)
		{
			callback(pCur->m_PlayerID);
		}
	}
}

void CVoiceBanMgr::Clear()
{
	// Tie off the hash table entries.
	for( size_t i = 0; i < MAX_BANNED_PLAYERS_HASH; ++i )
		m_PlayerHash[i].m_pNext = m_PlayerHash[i].m_pPrev = &m_PlayerHash[i];
}

CVoiceBanMgr::BannedPlayer* CVoiceBanMgr::InternalFindPlayerSquelch(char const playerID[ PLAYERID_BUFFER_SIZE ])
{
	int index = HashPlayerID(playerID);
	BannedPlayer *pListHead = &m_PlayerHash[index];
	for(BannedPlayer *pCur=pListHead->m_pNext; pCur != pListHead; pCur=pCur->m_pNext)
	{
		if(memcmp(playerID, pCur->m_PlayerID, sizeof( pCur->m_PlayerID ) ) == 0)
			return pCur;
	}

	return nullptr;
}

CVoiceBanMgr::BannedPlayer* CVoiceBanMgr::AddBannedPlayer(char const playerID[ PLAYERID_BUFFER_SIZE ])
{
	BannedPlayer *pNew = new( std::nothrow ) BannedPlayer;
	if(!pNew)
		return nullptr;

	int index = HashPlayerID(playerID);
	memcpy(pNew->m_PlayerID, playerID, sizeof( pNew->m_PlayerID ) );
	pNew->m_pNext = &m_PlayerHash[index];
	pNew->m_pPrev = m_PlayerHash[index].m_pPrev;
	pNew->m_pPrev->m_pNext = pNew->m_pNext->m_pPrev = pNew;
	return pNew;
}
