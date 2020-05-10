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
#ifndef GAME_SERVER_NODES_CQUEUE_H
#define GAME_SERVER_NODES_CQUEUE_H

#include "NodeConstants.h"

//=========================================================
// CQueue - first in, first out.
//=========================================================
class CQueue
{
public:

	CQueue( void );// constructor
	inline int Full( void ) { return ( m_cSize == MAX_STACK_NODES ); }
	inline int Empty( void ) { return ( m_cSize == 0 ); }
	//inline int Tail ( void ) { return ( m_queue[ m_tail ] ); }
	inline int Size( void ) { return ( m_cSize ); }
	void Insert( int, float );
	int Remove( float & );

private:
	int	m_cSize;
	struct tag_QUEUE_NODE
	{
		int   Id;
		float Priority;
	} m_queue[ MAX_STACK_NODES ];
	int m_head;
	int m_tail;
};

#endif //GAME_SERVER_NODES_CQUEUE_H