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
#ifndef GAME_SHARED_MATERIALS_MATERIALS_H
#define GAME_SHARED_MATERIALS_MATERIALS_H

#include "MaterialsConst.h"

#include "CMaterialsList.h"

extern CMaterialsList g_MaterialsList;

/**
*	Used by the engine.
*/
char PM_FindTextureType( const char* pszName );

namespace mat
{
/**
*	Gets the material data for the given type.
*	@param chTextureType Texture type. @see CharTexType
*	@param iBulletType Bullet type. @see Bullet
*	@param[ out ] flVol Volume.
*	@param[ out ] flVolBar Crowbar hit volume.
*	@param[ out ] flAttn Attenuation.
*	@param[ out ] ppszSounds List of sounds.
*	@param[ out ] uiCount Number of sounds placed in ppszSounds.
*	@return true if the sound should be played, false otherwise.
*/
bool GetMaterialForType( const char chTextureType, const int iBulletType, float& flVol, float& flVolBar, float& flAttn, const char** ppszSounds, size_t& uiCount );

/**
*	Gets the step type for the given texture type.
*	@param chTextureType Texture type.
*	@return Step type.
*/
StepTexType MapTextureTypeStepType( const CharTexType chTextureType );

inline StepTexType MapTextureTypeStepType( const char chTextureType )
{
	return MapTextureTypeStepType( static_cast<CharTexType>( chTextureType ) );
}

/**
*	@return Whether there's a grass texture.
*/
bool IsThereGrassTexture();
}

#endif //GAME_SHARED_MATERIALS_MATERIALS_H