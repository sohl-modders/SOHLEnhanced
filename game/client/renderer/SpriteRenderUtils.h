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
#ifndef GAME_CLIENT_RENDERER_SPRITERENDERUTILS_H
#define GAME_CLIENT_RENDERER_SPRITERENDERUTILS_H

/**
*	Adjusts texture coordinates and width and height according to a rectangle.
*	@param[ in ] iWidth Image width.
*	@param[ in ] iHeight Image height.
*	@param[ in, out ] pfLeft Left texture coordinate.
*	@param[ in, out ] pfRight Right texture coordinate.
*	@param[ in, out ] pfTop Top texture coordinate.
*	@param[ in, out ] pfBottom Bottom texture coordinate.
*	@param[ in, out ] pw New width. Should have a valid value beforehand.
*	@param[ in, out ] ph New height. Should have a valid value beforehand.
*	@param[ in ] prcSubRect Optional. Rectangle to adjust with.
*/
void AdjustSubRect( const int iWidth, const int iHeight, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom, int *pw, int *ph, const wrect_t *prcSubRect );

/**
*	Callback to compute the X and Y coordinates for a sprite. Needed to support Software mode.
*/
class ITriCoordFallback
{
public:
	/**
	*	Calculates the X and Y coordinates.
	*	@param hSprite Sprite whose coordinates are being computed.
	*	@param frame Sprite frame.
	*	@param rect Rect that defines the area in the sprite being drawn.
	*	@param flScale Sprite scale.
	*	@param[ out ] x X coordinate.
	*	@param[ out ] y Y coordinate.
	*/
	virtual void Calculate( HSPRITE hSprite, const int frame, const wrect_t& rect, const float flScale, int& x, int& y ) = 0;
};


/**
*	Draws a scaled sprite.
*	@param hSprite Sprite to draw.
*	@param frame Sprite frame to draw.
*	@param r Red color. [ 0, 255 ].
*	@param g Green color. [ 0, 255 ].
*	@param b Blue color. [ 0, 255 ].
*	@param a Alpha value. [ 0, 255 ].
*	@param renderMode Render mode.
*	@param flScale Scale.
*	@param callback Callback used to calculate sprite coordinates.
*	@param pSpriteRect Optional. Subrect to draw. 
*/
void Tri_DrawScaledSprite( HSPRITE hSprite, int frame, int r, int g, int b, int a, const RenderMode renderMode, const float flScale, ITriCoordFallback& callback, const wrect_t* pSpriteRect = nullptr );

#endif //GAME_CLIENT_RENDERER_SPRITERENDERUTILS_H