/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef GAME_SERVER_TEMPENTITY_H
#define GAME_SERVER_TEMPENTITY_H

/**
*	@file
*	Contains functions that wrap SVC_TEMPENTITY network messages.
*	@see SVC_TEMPENTITY
*/

/**
*	@see TE_BEAMPOINTS
*/
void BeamPoints(
	const Vector& origin, const Vector& vecStartPos, const Vector& vecEndPos,
	const int iSpriteModel, const int iFrame, const float flFrameRate, const float flLife, const int iWidth, const int iNoise,
	const Vector& vecColor, const float flBrightness, const int iSpeed );

/**
*	@see TE_STREAK_SPLASH
*/
void StreakSplash( const Vector &origin, const Vector &direction, int color, int count, int speed, int velocityRange );

/**
*	@see TE_SPRITE_SPRAY
*/
void SpriteSpray( const Vector &position, const Vector &direction, int spriteModel, int count );

#endif //GAME_SERVER_TEMPENTITY_H