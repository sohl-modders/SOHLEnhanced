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
#ifndef GAME_SERVER_BUTTONSOUNDS_H
#define GAME_SERVER_BUTTONSOUNDS_H

/**
*	Get string of button sound number.
*/
const char* ButtonSound( int sound );

/**
*	Get string of door move sound number.
*/
const char* DoorMoveSound( const int iSound );

/**
*	Get string of door stop sound number.
*/
const char* DoorStopSound( const int iSound );

/**
*	Get string of locked sentence sound number.
*/
const char* LockedSentence( const int iSound );

/**
*	Get string of unlocked sentence sound number.
*/
const char* UnlockedSentence( const int iSound );

// sounds that doors and buttons make when locked/unlocked
struct locksound_t
{
	string_t	sLockedSound;		// sound a door makes when it's locked
	string_t	sLockedSentence;	// sentence group played when door is locked
	string_t	sUnlockedSound;		// sound a door makes when it's unlocked
	string_t	sUnlockedSentence;	// sentence group played when door is unlocked

	int		iLockedSentence;		// which sentence in sentence group to play next
	int		iUnlockedSentence;		// which sentence in sentence group to play next

	float	flwaitSound;			// time delay between playing consecutive 'locked/unlocked' sounds
	float	flwaitSentence;			// time delay between playing consecutive sentences
	bool	bEOFLocked;				// true if hit end of list of locked sentences
	bool	bEOFUnlocked;			// true if hit end of list of unlocked sentences
};

void PlayLockSounds( CBaseEntity* pEntity, locksound_t *pls, const bool bLocked, const bool bButton );

#endif //GAME_SERVER_BUTTONSOUNDS_H