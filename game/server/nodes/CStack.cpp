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

#include "CStack.h"

//=========================================================
// CStack Constructor
//=========================================================
CStack::CStack( void )
{
	m_level = 0;
}

//=========================================================
// pushes a value onto the stack
//=========================================================
void CStack::Push( int value )
{
	if( m_level >= MAX_STACK_NODES )
	{
		printf( "Error!\n" );
		return;
	}
	m_stack[ m_level ] = value;
	m_level++;
}

//=========================================================
// pops a value off of the stack
//=========================================================
int CStack::Pop( void )
{
	if( m_level <= 0 )
		return -1;

	m_level--;
	return m_stack[ m_level ];
}

//=========================================================
// returns the value on the top of the stack
//=========================================================
int CStack::Top( void )
{
	return m_stack[ m_level - 1 ];
}

//=========================================================
// copies every element on the stack into an array LIFO 
//=========================================================
void CStack::CopyToArray( int *piArray )
{
	int	i;

	for( i = 0; i < m_level; i++ )
	{
		piArray[ i ] = m_stack[ i ];
	}
}