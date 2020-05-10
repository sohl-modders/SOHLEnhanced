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
#ifndef GAME_SERVER_NODES_CNODEVIEWER_H
#define GAME_SERVER_NODES_CNODEVIEWER_H

//=========================================================
// CNodeViewer - Draws a graph of the shorted path from all nodes
// to current location (typically the player).  It then draws
// as many connects as it can per frame, trying not to overflow the buffer
//=========================================================
class CNodeViewer : public CBaseEntity
{
public:
	DECLARE_CLASS( CNodeViewer, CBaseEntity );
	DECLARE_DATADESC();

	void Spawn( void ) override;

	int m_iBaseNode;
	int m_iDraw;
	int	m_nVisited;
	int m_aFrom[ 128 ];
	int m_aTo[ 128 ];
	int m_iHull;
	int m_afNodeType;
	Vector m_vecColor;

	void FindNodeConnections( int iNode );
	void AddNode( int iFrom, int iTo );
	void DrawThink( void );
};

#endif //GAME_SERVER_NODES_CNODEVIEWER_H