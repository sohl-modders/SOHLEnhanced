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
#ifndef GAME_SERVER_NODES_CLINK_H
#define GAME_SERVER_NODES_CLINK_H

//=========================================================
// CLink - A link between 2 nodes
//=========================================================
#define		bits_LINK_SMALL_HULL	( 1 << 0 )// headcrab box can fit through this connection
#define		bits_LINK_HUMAN_HULL	( 1 << 1 )// player box can fit through this connection
#define		bits_LINK_LARGE_HULL	( 1 << 2 )// big box can fit through this connection
#define		bits_LINK_FLY_HULL		( 1 << 3 )// a flying big box can fit through this connection
#define		bits_LINK_DISABLED		( 1 << 4 )// link is not valid when the set

class CLink
{
public:
	int		m_iSrcNode;// the node that 'owns' this link ( keeps us from having to make reverse lookups )
	int		m_iDestNode;// the node on the other end of the link. 

	entvars_t	*m_pLinkEnt;// the entity that blocks this connection (doors, etc)

							// m_szLinkEntModelname is not necessarily NULL terminated (so we can store it in a more alignment-friendly 4 bytes)
	//TODO: what if there are more than 1000 brush models? - Solokiller
	char	m_szLinkEntModelname[ 4 ];// the unique name of the brush model that blocks the connection (this is kept for save/restore)

	int		m_afLinkInfo;// information about this link
	float	m_flWeight;// length of the link line segment
};

#endif //GAME_SERVER_NODES_CLINK_H