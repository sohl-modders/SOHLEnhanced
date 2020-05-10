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
#ifndef GAME_SERVER_CSTUDIOBLENDING_H
#define GAME_SERVER_CSTUDIOBLENDING_H

struct Matrix3x4;
struct model_t;

/**
*	Studio model blending. - Solokiller
*/
class CStudioBlending final
{
public:
	CStudioBlending() = default;

	bool Initialize( int version, sv_blending_interface_t** ppInterface, server_studio_api_t* pStudio, Matrix3x4* pRotationMatrix, Matrix3x4* pBoneTransform );

	void StudioSetupBones( model_t*			pModel,
						   float			frame,
						   int				sequence,
						   const Vector&	angles,
						   const Vector&	origin,
						   const byte*		pcontroller,
						   const byte*		pblending,
						   int				iBone,
						   const edict_t*	pEdict );

private:
	Matrix3x4* m_pRotationMatrix = nullptr;

	//MAXSTUDIOBONES elements in array.
	Matrix3x4* m_pBoneTransform = nullptr;

private:
	CStudioBlending( const CStudioBlending& ) = delete;
	CStudioBlending& operator=( const CStudioBlending& ) = delete;
};

extern server_studio_api_t IEngineStudio;

extern CStudioBlending g_StudioBlending;

#endif //GAME_SERVER_CSTUDIOBLENDING_H
