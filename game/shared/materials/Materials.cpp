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
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "Materials.h"

// Texture names
CMaterialsList g_MaterialsList;

char PM_FindTextureType( const char* pszName )
{
	return g_MaterialsList.FindTextureType( pszName );
}

namespace mat
{
bool GetMaterialForType( const char chTextureType, const int iBulletType, float& flVol, float& flVolBar, float& flAttn, const char** ppszSounds, size_t& uiCount )
{
	flAttn = ATTN_NORM;

	switch( chTextureType )
	{
	default:
	case CHAR_TEX_CONCRETE: flVol = 0.9; flVolBar = 0.6;
		ppszSounds[ 0 ] = "player/pl_step1.wav";
		ppszSounds[ 1 ] = "player/pl_step2.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_METAL: flVol = 0.9; flVolBar = 0.3;
		ppszSounds[ 0 ] = "player/pl_metal1.wav";
		ppszSounds[ 1 ] = "player/pl_metal2.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_DIRT:	flVol = 0.9; flVolBar = 0.1;
		ppszSounds[ 0 ] = "player/pl_dirt1.wav";
		ppszSounds[ 1 ] = "player/pl_dirt2.wav";
		ppszSounds[ 2 ] = "player/pl_dirt3.wav";
		uiCount = 3;
		break;
	case CHAR_TEX_VENT:	flVol = 0.5; flVolBar = 0.3;
		ppszSounds[ 0 ] = "player/pl_duct1.wav";
		ppszSounds[ 1 ] = "player/pl_duct1.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_GRATE: flVol = 0.9; flVolBar = 0.5;
		ppszSounds[ 0 ] = "player/pl_grate1.wav";
		ppszSounds[ 1 ] = "player/pl_grate4.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_TILE:	flVol = 0.8; flVolBar = 0.2;
		ppszSounds[ 0 ] = "player/pl_tile1.wav";
		ppszSounds[ 1 ] = "player/pl_tile3.wav";
		ppszSounds[ 2 ] = "player/pl_tile2.wav";
		ppszSounds[ 3 ] = "player/pl_tile4.wav";
		uiCount = 4;
		break;
	case CHAR_TEX_SLOSH: flVol = 0.9; flVolBar = 0.0;
		ppszSounds[ 0 ] = "player/pl_slosh1.wav";
		ppszSounds[ 1 ] = "player/pl_slosh3.wav";
		ppszSounds[ 2 ] = "player/pl_slosh2.wav";
		ppszSounds[ 3 ] = "player/pl_slosh4.wav";
		uiCount = 4;
		break;
	case CHAR_TEX_WOOD: flVol = 0.9; flVolBar = 0.2;
		ppszSounds[ 0 ] = "debris/wood1.wav";
		ppszSounds[ 1 ] = "debris/wood2.wav";
		ppszSounds[ 2 ] = "debris/wood3.wav";
		uiCount = 3;
		break;
	case CHAR_TEX_GLASS:
	case CHAR_TEX_COMPUTER:
		flVol = 0.8; flVolBar = 0.2;
		ppszSounds[ 0 ] = "debris/glass1.wav";
		ppszSounds[ 1 ] = "debris/glass2.wav";
		ppszSounds[ 2 ] = "debris/glass3.wav";
		uiCount = 3;
		break;
	case CHAR_TEX_FLESH:
		if( iBulletType == BULLET_PLAYER_CROWBAR )
			return false; // crowbar already makes this sound
		flVol = 1.0;	flVolBar = 0.2;
		ppszSounds[ 0 ] = "weapons/bullet_hit1.wav";
		ppszSounds[ 1 ] = "weapons/bullet_hit2.wav";
		flAttn = 1.0;
		uiCount = 2;
		break;

	case CHAR_TEX_SNOW:
		flVol = 0.8;
		flVolBar = 0.7;
		ppszSounds[ 0 ] = "player/pl_snow1.wav";
		ppszSounds[ 1 ] = "player/pl_snow2.wav";
		ppszSounds[ 2 ] = "player/pl_snow3.wav";
		ppszSounds[ 3 ] = "player/pl_snow4.wav";

		uiCount = 4;
		break;
	}

	return true;
}

StepTexType MapTextureTypeStepType( const CharTexType chTextureType )
{
	switch( chTextureType )
	{
	default:
	case CHAR_TEX_CONCRETE:		return STEP_CONCRETE;
	case CHAR_TEX_METAL:		return STEP_METAL;
	case CHAR_TEX_DIRT:			return STEP_DIRT;
	case CHAR_TEX_VENT:			return STEP_VENT;
	case CHAR_TEX_GRATE:		return STEP_GRATE;
	case CHAR_TEX_TILE:			return STEP_TILE;
	case CHAR_TEX_SLOSH:		return STEP_SLOSH;
	case CHAR_TEX_SNOW:			return STEP_SNOW;
	}
}

bool IsThereGrassTexture()
{
	return g_MaterialsList.FindTextureByType( CMaterialsList::INVALID_TEX_INDEX, CHAR_TEX_GRASS ) != CMaterialsList::INVALID_TEX_INDEX;
}
}