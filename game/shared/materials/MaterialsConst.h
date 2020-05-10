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
#ifndef GAME_SHARED_MATERIALS_MATERIALSCONST_H
#define GAME_SHARED_MATERIALS_MATERIALSCONST_H

/**
*	Max number of textures loaded.
*/
#define CTEXTURESMAX		512

/**
*	Now matches the maximum name length of a WAD lump. - Solokiller
*/
#define CBTEXTURENAMEMAX	16

/**
*	Texture types.
*/
enum CharTexType : char
{
	CHAR_TEX_CONCRETE	= 'C',
	CHAR_TEX_METAL		= 'M',
	CHAR_TEX_DIRT		= 'D',
	CHAR_TEX_VENT		= 'V',
	CHAR_TEX_GRATE		= 'G',
	CHAR_TEX_TILE		= 'T',
	CHAR_TEX_SLOSH		= 'S',
	CHAR_TEX_WOOD		= 'W',
	CHAR_TEX_COMPUTER	= 'P',
	CHAR_TEX_GLASS		= 'Y',
	CHAR_TEX_FLESH		= 'F',
	CHAR_TEX_SNOW		= 'N',
	CHAR_TEX_GRASS		= 'X', //TODO: needs to be handled in sound code - Solokiller
};

/**
*	Step types. Note, not all step types have a texture type.
*/
enum StepTexType
{
	/**
	*	Default step sound.
	*/
	STEP_CONCRETE	= 0,

	/**
	*	Metal floor.
	*/
	STEP_METAL		= 1,

	/**
	*	Dirt, sand, rock.
	*/
	STEP_DIRT		= 2,

	/**
	*	Ventillation duct.
	*/
	STEP_VENT		= 3,

	/**
	*	Metal grating.
	*/
	STEP_GRATE		= 4,

	/**
	*	Floor tiles.
	*/
	STEP_TILE		= 5,

	/**
	*	Shallow liquid puddle.
	*/
	STEP_SLOSH		= 6,

	/**
	*	Wading in liquid.
	*/
	STEP_WADE		= 7,

	/**
	*	Climbing ladder.
	*/
	STEP_LADDER		= 8,

	/**
	*	Walking on snow.
	*/
	STEP_SNOW		= 9,
};

#endif //GAME_SHARED_MATERIALS_MATERIALSCONST_H
