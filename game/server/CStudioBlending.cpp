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
#include "extdll.h"
#include "util.h"

#include "r_studioint.h"
#include "com_model.h"
#include "studio.h"
#include "studio/StudioUtils.h"

#include "CStudioBlending.h"

server_studio_api_t IEngineStudio;

CStudioBlending g_StudioBlending;

static sv_blending_interface_t blending_functions = 
{
	SV_BLENDING_INTERFACE_VERSION,

	[]( model_t*		pModel,
		float			frame,
		int				sequence,
		const Vector&	angles,
		const Vector&	origin,
		const byte*		pcontroller,
		const byte*		pblending,
		int				iBone,
		const edict_t*	pEdict )
	{
		g_StudioBlending.StudioSetupBones( pModel, frame, sequence, angles, origin, pcontroller, pblending, iBone, pEdict );
	}
};

bool CStudioBlending::Initialize( int version, sv_blending_interface_t** ppInterface, server_studio_api_t* pStudio, Matrix3x4* pRotationMatrix, Matrix3x4* pBoneTransform )
{
	if( !ppInterface || !pStudio || !pRotationMatrix || !pBoneTransform || version != SV_BLENDING_INTERFACE_VERSION )
		return false;

	*ppInterface = &blending_functions;

	IEngineStudio = *pStudio;

	m_pRotationMatrix = pRotationMatrix;
	m_pBoneTransform = pBoneTransform;

	return true;
}

void CStudioBlending::StudioSetupBones( model_t*		pModel,
										float			frame,
										int				sequence,
										const Vector&	angles,
										const Vector&	origin,
										const byte*		pcontroller,
										const byte*		pblending,
										int				iBone,
										const edict_t*	pEdict )
{
	//TODO: implement default bone setup. - Solokiller
}
