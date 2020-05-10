//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// com_weapons.h
// Shared weapons common function prototypes
#if !defined( COM_WEAPONSH )
#define COM_WEAPONSH
#ifdef _WIN32
#pragma once
#endif

#include "Exports.h"

struct local_state_t;

void			COM_Log( const char* const pszFile, const char* const pszFormat, ... );
int				CL_IsDead();

int				HUD_GetWeaponAnim();
void			HUD_SendWeaponAnim( int iAnim, int body, int force );
void			HUD_PlaySound( const char* const pszSound, float volume );

extern cvar_t *cl_lw;

extern int g_runfuncs;
extern Vector v_angles;
extern Vector v_client_aimangles;
extern float g_lastFOV;
extern local_state_t *g_finalstate;

void CL_SetupServerSupport();

#endif