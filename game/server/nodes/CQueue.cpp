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
#include <cstdio>

#include "CQueue.h"

//=========================================================
// CQueue constructor
//=========================================================
CQueue::CQueue( void )
{
	m_cSize = 0;
	m_head = 0;
	m_tail = -1;
}

//=========================================================
// inserts a value into the queue
//=========================================================
void CQueue::Insert( int iValue, float fPriority )
{

	if( Full() )
	{
		printf( "Queue is full!\n" );
		return;
	}

	m_tail++;

	if( m_tail == MAX_STACK_NODES )
	{//wrap around
		m_tail = 0;
	}

	m_queue[ m_tail ].Id = iValue;
	m_queue[ m_tail ].Priority = fPriority;
	m_cSize++;
}

//=========================================================
// removes a value from the queue (FIFO)
//=========================================================
int CQueue::Remove( float &fPriority )
{
	if( m_head == MAX_STACK_NODES )
	{// wrap
		m_head = 0;
	}

	m_cSize--;
	fPriority = m_queue[ m_head ].Priority;
	return m_queue[ m_head++ ].Id;
}