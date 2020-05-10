/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
#ifndef GAME_CLIENT_HL_HL_WEAPONS_H
#define GAME_CLIENT_HL_HL_WEAPONS_H

class CBasePlayerWeapon;
class CBasePlayer;
class Vector;

/**
*	Returns the last position that we stored for egon beam endpoint.
*	@param[ out ] vecOrigin Will contain the origin.
*/
void HUD_GetLastOrg( Vector& vecOrigin );

/**
*	Remember our exact predicted origin so we can draw the egon to the right position.
*/
void HUD_SetLastOrg();

/**
*	For debugging, draw a box around a player made out of particles.
*	@param pPlayer Player.
*	@param vecMins Minimum bounds.
*	@param vecMaxs Maximum bounds.
*	@param life Particle lifetime, in seconds.
*	@param r Red color.
*	@param g Green color.
*	@param b Blue color.
*/
void UTIL_ParticleBox( const CBasePlayer* const pPlayer, const Vector& vecMins, const Vector& vecMaxs, float life, unsigned char r, unsigned char g, unsigned char b );

/**
*	For debugging, draw boxes for other collidable players.
*/
void UTIL_ParticleBoxes();

/**
*	For debugging, draw a line made out of particles.
*	@param vecStart Starting point.
*	@param vecEnd End point.
*	@param life Particle lifetime, in seconds.
*	@param r Red color.
*	@param g Green color.
*	@param b Blue color.
*/
void UTIL_ParticleLine( const Vector& vecStart, const Vector& vecEnd, float life, unsigned char r, unsigned char g, unsigned char b );

#endif //GAME_CLIENT_HL_HL_WEAPONS_H