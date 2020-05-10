//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VOICE_BANMGR_H
#define VOICE_BANMGR_H
#ifdef _WIN32
#pragma once
#endif

/**
*	Hash a player ID to a byte.
*	@param playerID Player ID.
*	@return Hash.
*/
unsigned char HashPlayerID( const char playerID[ PLAYERID_BUFFER_SIZE ] );

/**
*	This class manages the (persistent) list of squelched players.
*/
class CVoiceBanMgr final
{
public:
	/**
	*	The hash array size is equal to the largest hash value that HashPlayerID can return.
	*	Note that HashPlayerID will frequently overflow its hash variable, constraining it to [ 0, 255 ].
	*/
	static const size_t MAX_BANNED_PLAYERS_HASH = 256 * sizeof( unsigned char );

	//Sanity check to prevent array index out of bounds issues.
	static_assert( sizeof( unsigned char ) == sizeof( decltype( HashPlayerID( "" ) ) ), "HashPlayerID return type has unexpected size!" );

	using ForEachCallback = void ( * )( const char id[ PLAYERID_BUFFER_SIZE ] );

public:
	/**
	*	Constructor.
	*/
	CVoiceBanMgr();

	/**
	*	Destructor.
	*/
	~CVoiceBanMgr();

	/**
	*	Loads the list of squelched players from disk.
	*	@param pszGameDir Game directory.
	*/
	bool Init( const char* const pszGameDir );

	/**
	*	Removes all bans.
	*/
	void Term();

	/**
	*	Saves the state into voice_squelch.dt.
	*	@param pszGameDir Game directory.
	*/
	void SaveState( const char* const pszGameDir );

	/**
	*	Gets whether the given player is banned.
	*	@param playerID Player unique ID.
	*	@return true if the player is banned, false otherwise.
	*/
	bool GetPlayerBan( const char playerID[ PLAYERID_BUFFER_SIZE ] );

	/**
	*	Sets whether the given player is banned.
	*	@param playerID Player unique ID.
	*	@param bSquelch Whether the player should be banned.
	*/
	void SetPlayerBan( const char playerID[ PLAYERID_BUFFER_SIZE ], bool bSquelch );

	/**
	*	Call your callback for each banned player.
	*	@param callback Callback.
	*/
	void ForEachBannedPlayer( ForEachCallback callback );

protected:

	class BannedPlayer final
	{
	public:
		char			m_PlayerID[ PLAYERID_BUFFER_SIZE ];
		BannedPlayer	*m_pPrev, *m_pNext;
	};

	/**
	*	Tie off the hash table entries.
	*/
	void Clear();

	/**
	*	Finds a player's ban entry.
	*	@param playerID Player unique ID.
	*	@return Ban entry, or null if the player isn't banned.
	*/
	BannedPlayer* InternalFindPlayerSquelch( const char playerID[ PLAYERID_BUFFER_SIZE ] );

	/**
	*	Adds a player to the ban list.
	*	@param playerID Player unique ID.
	*	@return Ban entry.
	*/
	BannedPlayer* AddBannedPlayer( const char playerID[ PLAYERID_BUFFER_SIZE ] );


protected:

	BannedPlayer m_PlayerHash[ MAX_BANNED_PLAYERS_HASH ];
};


#endif // VOICE_BANMGR_H
