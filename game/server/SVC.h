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
#ifndef GAME_SERVER_SVC_H
#define GAME_SERVER_SVC_H

/**
*	@file
*	Server commands to the client.
*/

//TODO: add all of the commands. - Solokiller
enum SVC
{
	/**
	*	[string] stuffed into client's console buffer
	*	The string should be \n terminated
	*	Taken from WinQuake. - Solokiller
	*/
	SVC_STUFFTEXT		= 9,
	SVC_TEMPENTITY		= 23,
	SVC_INTERMISSION	= 30,
	SVC_CDTRACK			= 32,
	SVC_WEAPONANIM		= 35,
	SVC_ROOMTYPE		= 37,
	SVC_DIRECTOR		= 51,
};

#endif //GAME_SERVER_SVC_H