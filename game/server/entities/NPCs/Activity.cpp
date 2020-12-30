/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#include "Activity.h"

#define ActivityACT( a ) { a, #a }

activity_map_t activity_map[] =
{
	ActivityACT(ACT_IDLE),
	ActivityACT(ACT_GUARD),
	ActivityACT(ACT_WALK),
	ActivityACT(ACT_RUN),
	ActivityACT(ACT_FLY),
	ActivityACT(ACT_SWIM),
	ActivityACT(ACT_HOP),
	ActivityACT(ACT_LEAP),
	ActivityACT(ACT_FALL),
	ActivityACT(ACT_LAND),
	ActivityACT(ACT_STRAFE_LEFT),
	ActivityACT(ACT_STRAFE_RIGHT),
	ActivityACT(ACT_ROLL_LEFT),
	ActivityACT(ACT_ROLL_RIGHT),
	ActivityACT(ACT_TURN_LEFT),
	ActivityACT(ACT_TURN_RIGHT),
	ActivityACT(ACT_CROUCH),
	ActivityACT(ACT_CROUCHIDLE),
	ActivityACT(ACT_STAND),
	ActivityACT(ACT_USE),
	ActivityACT(ACT_SIGNAL1),
	ActivityACT(ACT_SIGNAL2),
	ActivityACT(ACT_SIGNAL3),
	ActivityACT(ACT_TWITCH),
	ActivityACT(ACT_COWER),
	ActivityACT(ACT_SMALL_FLINCH),
	ActivityACT(ACT_BIG_FLINCH),
	ActivityACT(ACT_RANGE_ATTACK1),
	ActivityACT(ACT_RANGE_ATTACK2),
	ActivityACT(ACT_MELEE_ATTACK1),
	ActivityACT(ACT_MELEE_ATTACK2),
	ActivityACT(ACT_RELOAD),
	ActivityACT(ACT_ARM),
	ActivityACT(ACT_DISARM),
	ActivityACT(ACT_EAT),
	ActivityACT(ACT_DIESIMPLE),
	ActivityACT(ACT_DIEBACKWARD),
	ActivityACT(ACT_DIEFORWARD),
	ActivityACT(ACT_DIEVIOLENT),
	ActivityACT(ACT_BARNACLE_HIT),
	ActivityACT(ACT_BARNACLE_PULL),
	ActivityACT(ACT_BARNACLE_CHOMP),
	ActivityACT(ACT_BARNACLE_CHEW),
	ActivityACT(ACT_SLEEP),
	ActivityACT(ACT_INSPECT_FLOOR),
	ActivityACT(ACT_INSPECT_WALL),
	ActivityACT(ACT_IDLE_ANGRY),
	ActivityACT(ACT_WALK_HURT),
	ActivityACT(ACT_RUN_HURT),
	ActivityACT(ACT_HOVER),
	ActivityACT(ACT_GLIDE),
	ActivityACT(ACT_FLY_LEFT),
	ActivityACT(ACT_FLY_RIGHT),
	ActivityACT(ACT_DETECT_SCENT),
	ActivityACT(ACT_SNIFF),
	ActivityACT(ACT_BITE),
	ActivityACT(ACT_THREAT_DISPLAY),
	ActivityACT(ACT_FEAR_DISPLAY),
	ActivityACT(ACT_EXCITED),
	ActivityACT(ACT_SPECIAL_ATTACK1),
	ActivityACT(ACT_SPECIAL_ATTACK2),
	ActivityACT(ACT_COMBAT_IDLE),
	ActivityACT(ACT_WALK_SCARED),
	ActivityACT(ACT_RUN_SCARED),
	ActivityACT(ACT_VICTORY_DANCE),
	ActivityACT(ACT_DIE_HEADSHOT),
	ActivityACT(ACT_DIE_CHESTSHOT),
	ActivityACT(ACT_DIE_GUTSHOT),
	ActivityACT(ACT_DIE_BACKSHOT),
	ActivityACT(ACT_FLINCH_HEAD),
	ActivityACT(ACT_FLINCH_CHEST),
	ActivityACT(ACT_FLINCH_STOMACH),
	ActivityACT(ACT_FLINCH_LEFTARM),
	ActivityACT(ACT_FLINCH_RIGHTARM),
	ActivityACT(ACT_FLINCH_LEFTLEG),
	ActivityACT(ACT_FLINCH_RIGHTLEG),
	{ 0, nullptr }
};
