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
#ifndef GAME_SERVER_NODES_NODECONSTANTS_H
#define GAME_SERVER_NODES_NODECONSTANTS_H

//=========================================================
// DEFINE
//=========================================================
#define MAX_STACK_NODES	    100
#define	NO_NODE				-1
#define MAX_NODE_HULLS		4

#define		NODE_SMALL_HULL			0
#define		NODE_HUMAN_HULL			1
#define		NODE_LARGE_HULL			2
#define		NODE_FLY_HULL			3

#define bits_NODE_LAND      ( 1 << 0 )  // Land node, so nudge if necessary.
#define bits_NODE_AIR       ( 1 << 1 )  // Air node, don't nudge.
#define bits_NODE_WATER     ( 1 << 2 )  // Water node, don't nudge.
#define bits_NODE_GROUP_REALM (bits_NODE_LAND | bits_NODE_AIR | bits_NODE_WATER)

#define	HULL_STEP_SIZE 16// how far the test hull moves on each step
#define	NODE_HEIGHT	8	// how high to lift nodes off the ground after we drop them all (make stair/ramp mapping easier)

// to help eliminate node clutter by level designers, this is used to cap how many other nodes
// any given node is allowed to 'see' in the first stage of graph creation "LinkVisibleNodes()".
#define	MAX_NODE_INITIAL_LINKS	128
#define	MAX_NODES               1024

#endif //GAME_SERVER_NODES_NODECONSTANTS_H