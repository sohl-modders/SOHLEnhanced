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
#ifndef GAME_SERVER_NODES_CSTACK_H
#define GAME_SERVER_NODES_CSTACK_H

#include "NodeConstants.h"

//=========================================================
// CStack - last in, first out.
//=========================================================
class CStack
{
public:
	CStack( void );
	void	Push( int value );
	int		Pop( void );
	int		Top( void );
	int		Empty( void ) { return m_level == 0; }
	int		Size( void ) { return m_level; }
	void    CopyToArray( int *piArray );

private:
	int		m_stack[ MAX_STACK_NODES ];
	int		m_level;
};

#endif //GAME_SERVER_NODES_CSTACK_H