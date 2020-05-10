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
#ifndef GAME_SERVER_NODES_CTESTHULL_H
#define GAME_SERVER_NODES_CTESTHULL_H

/**
*	TestHull is a modelless clip hull that verifies reachable
*	nodes by walking from every node to each of it's connections
*/
class CTestHull : public CBaseMonster
{
public:
	DECLARE_CLASS( CTestHull, CBaseMonster );
	DECLARE_DATADESC();

	void Spawn() override;
	virtual int	ObjectCaps() const override { return CBaseMonster::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	void CallBuildNodeGraph();

private:
	void BuildNodeGraph();

public:
	void ShowBadNode();
	void PathFind();

private:
	Vector vecBadNodeOrigin;
};

#endif //GAME_SERVER_NODES_CTESTHULL_H