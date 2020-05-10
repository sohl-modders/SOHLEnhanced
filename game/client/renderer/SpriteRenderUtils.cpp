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
#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "r_studioint.h"

#include "SpriteRenderUtils.h"

extern engine_studio_api_t IEngineStudio;

void AdjustSubRect( const int iWidth, const int iHeight, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom, int *pw, int *ph, const wrect_t *prcSubRect )
{
	if( prcSubRect )
	{
		int iLeft = prcSubRect->left;
		int iRight = prcSubRect->right;
		if( prcSubRect->left < iRight )
		{
			int iTop = prcSubRect->top;
			int iBottom = prcSubRect->bottom;
			if( iTop < iBottom )
			{
				if( iLeft < 0 )
					iLeft = 0;
				if( *pw <= iRight )
					iRight = *pw;
				if( iLeft < iRight )
				{
					if( iTop < 0 )
						iTop = 0;
					if( *ph <= iBottom )
						iBottom = *ph;
					if( iTop < iBottom )
					{
						*pw = iRight - iLeft;
						*ph = iBottom - iTop;
						double flWidth = 1.0 / ( long double ) iWidth;
						*pfLeft = ( ( long double ) iLeft + 0.5 ) * flWidth;
						*pfRight = ( ( long double ) iRight - 0.5 ) * flWidth;
						double flHeight = 1.0 / ( long double ) iHeight;
						*pfTop = ( ( long double ) iTop + 0.5 ) * flHeight;
						*pfBottom = ( ( long double ) iBottom - 0.5 ) * flHeight;
					}
				}
			}
		}
	}
}

void Tri_DrawScaledSprite( HSPRITE hSprite, int frame, int r, int g, int b, int a, const RenderMode renderMode, const float flScale, ITriCoordFallback& callback, const wrect_t* pSpriteRect )
{
	if( hSprite == INVALID_HSPRITE )
	{
		gEngfuncs.Con_DPrintf( "Tri_DrawScaledSprite: hSprite is invalid!\n" );
		return;
	}

	const int iOrigWidth = gEngfuncs.pfnSPR_Width( hSprite, frame );
	const int iOrigHeight = gEngfuncs.pfnSPR_Height( hSprite, frame );

	wrect_t subRect;

	if( pSpriteRect )
	{
		subRect = *pSpriteRect;
	}
	else
	{
		subRect.left = subRect.top = 0;
		subRect.right = iOrigWidth;
		subRect.bottom = iOrigHeight;
	}

	int x, y;

	//Fallback on Software; no scaling or render mode support. - Solokiller
	if( !IEngineStudio.IsHardware() )
	{
		//Always scale 1. - Solokiller
		callback.Calculate( hSprite, frame, subRect, 1.0f, x, y );

		gEngfuncs.pfnSPR_Set( hSprite, r, g, b );

		gEngfuncs.pfnSPR_DrawHoles( frame, x, y, pSpriteRect );

		return;
	}

	wrect_t rect;

	//Trim a pixel border around it, since it blends. - Solokiller
	rect.left = subRect.left * flScale + ( flScale - 1 );
	rect.top = subRect.top * flScale + ( flScale - 1 );
	rect.right = subRect.right * flScale - ( flScale - 1 );
	rect.bottom = subRect.bottom * flScale - ( flScale - 1 );

	const int iWidth = iOrigWidth * flScale;
	const int iHeight = iOrigHeight * flScale;

	callback.Calculate( hSprite, frame, rect, flScale, x, y );

	model_t* pCrosshair = const_cast<model_t*>( gEngfuncs.GetSpritePointer( hSprite ) );

	auto TriAPI = gEngfuncs.pTriAPI;

	TriAPI->SpriteTexture( pCrosshair, frame );

	TriAPI->Color4fRendermode( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f, renderMode );
	TriAPI->RenderMode( renderMode );

	float flLeft = 0;
	float flTop = 0;
	float flRight = 1.0;
	float flBottom = 1.0f;

	int iImgWidth = iWidth;
	int iImgHeight = iHeight;

	AdjustSubRect( iWidth, iHeight, &flLeft, &flRight, &flTop, &flBottom, &iImgWidth, &iImgHeight, &rect );

	TriAPI->Begin( TriangleMode::TRI_QUADS );

	TriAPI->TexCoord2f( flLeft, flTop );
	TriAPI->Vertex3f( x, y, 0 );

	TriAPI->TexCoord2f( flRight, flTop );
	TriAPI->Vertex3f( x + iImgWidth, y, 0 );

	TriAPI->TexCoord2f( flRight, flBottom );
	TriAPI->Vertex3f( x + iImgWidth, y + iImgHeight, 0 );

	TriAPI->TexCoord2f( flLeft, flBottom );
	TriAPI->Vertex3f( x, y + iImgHeight, 0 );

	TriAPI->End();
}