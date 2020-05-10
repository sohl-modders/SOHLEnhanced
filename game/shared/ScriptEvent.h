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
#ifndef GAME_SHARED_SCRIPTEVENT_H
#define GAME_SHARED_SCRIPTEVENT_H

/**
*	Common animation events.
*/
enum ScriptEvent
{
	/**
	*	Character is now dead.
	*/
	SCRIPT_EVENT_DEAD					= 1000,

	/**
	*	Does not allow interrupt.
	*/
	SCRIPT_EVENT_NOINTERRUPT			= 1001,

	/**
	*	Will allow interrupt.
	*/
	SCRIPT_EVENT_CANINTERRUPT			= 1002,

	/**
	*	Event now fires.
	*/
	SCRIPT_EVENT_FIREEVENT				= 1003,

	/**
	*	Play named wave file (on CHAN_BODY).
	*/
	SCRIPT_EVENT_SOUND					= 1004,

	/**
	*	Play named sentence.
	*/
	SCRIPT_EVENT_SENTENCE				= 1005,

	/**
	*	Leave the character in air at the end of the sequence (don't find the floor).
	*/
	SCRIPT_EVENT_INAIR					= 1006,

	/**
	*	Set the animation by name after the sequence completes.
	*/
	SCRIPT_EVENT_ENDANIMATION			= 1007,

	/**
	*	Play named wave file (on CHAN_VOICE).
	*/
	SCRIPT_EVENT_SOUND_VOICE			= 1008,

	/**
	*	Play sentence group 25% of the time.
	*/
	SCRIPT_EVENT_SENTENCE_RND1			= 1009,

	/**
	*	Bring back to life (for life/death sequences).
	*/
	SCRIPT_EVENT_NOT_DEAD				= 1010,

	/*
	*	Client events.
	*/

	/**
	*	Show muzzleflash at attachment 0.
	*/
	SCRIPT_EVENT_MUZZLEFLASH_ATTACH0	= 5001,

	/**
	*	Show muzzleflash at attachment 1.
	*/
	SCRIPT_EVENT_MUZZLEFLASH_ATTACH1	= 5011,

	/**
	*	Show muzzleflash at attachment 2.
	*/
	SCRIPT_EVENT_MUZZLEFLASH_ATTACH2	= 5021,

	/**
	*	Show muzzleflash at attachment 3.
	*/
	SCRIPT_EVENT_MUZZLEFLASH_ATTACH3	= 5031,

	/**
	*	Show a spark at attachment 0.
	*/
	SCRIPT_EVENT_SPARK					= 5002,

	/**
	*	Play named sound at attachment 0.
	*/
	SCRIPT_EVENT_CLIENT_PLAYSOUND		= 5004,
};

#endif //GAME_SHARED_SCRIPTEVENT_H
