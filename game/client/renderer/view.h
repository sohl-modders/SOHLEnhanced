//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined ( VIEWH )
#define VIEWH 
#pragma once

extern Vector v_origin;
extern Vector v_angles;
extern Vector v_cl_angles;
extern Vector v_sim_org;
extern Vector v_lastAngles;
extern Vector v_client_aimangles;

void V_StartPitchDrift( void );
void V_StopPitchDrift( void );

#endif // !VIEWH