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
#ifndef GAME_SERVER_ENTITIES_ITEMS_CITEMSUIT_H
#define GAME_SERVER_ENTITIES_ITEMS_CITEMSUIT_H

#define SF_SUIT_SHORTLOGON		0x0001

/**
*	THE POWER SUIT
*	
*	The Suit provides 3 main functions: Protection, Notification and Augmentation.
*	Some functions are automatic, some require power.
*	The player gets the suit shortly after getting off the train in C1A0 and it stays
*	with him for the entire game.
*	
*	Protection
*	
*	Heat/Cold
*	When the player enters a hot/cold area, the heating/cooling indicator on the suit
*	will come on and the battery will drain while the player stays in the area.
*	After the battery is dead, the player starts to take damage.
*	This feature is built into the suit and is automatically engaged.
*	Radiation Syringe
*	This will cause the player to be immune from the effects of radiation for N seconds. Single use item.
*	Anti-Toxin Syringe
*	This will cure the player from being poisoned. Single use item.
*	Health
*	Small (1st aid kits, food, etc.)
*	Large (boxes on walls)
*	Armor
*	The armor works using energy to create a protective field that deflects a
*	percentage of damage projectile and explosive attacks. After the armor has been deployed,
*	it will attempt to recharge itself to full capacity with the energy reserves from the battery.
*	It takes the armor N seconds to fully charge.
*	
*	Notification (via the HUD)
*	
*	x	Health
*	x	Ammo
*	x	Automatic Health Care
*	Notifies the player when automatic healing has been engaged.
*	x	Geiger counter
*	Classic Geiger counter sound and status bar at top of HUD
*	alerts player to dangerous levels of radiation. This is not visible when radiation levels are normal.
*	x	Poison
*	Armor
*	Displays the current level of armor.
*	
*	Augmentation
*	
*	Reanimation (w/adrenaline)
*	Causes the player to come back to life after he has been dead for 3 seconds.
*	Will not work if player was gibbed. Single use.
*	Long Jump
*	Used by hitting the ??? key(s). Caused the player to further than normal.
*	SCUBA
*	Used automatically after picked up and after player enters the water.
*	Works for N seconds. Single use.
*	
*	Things powered by the battery
*	
*	Armor
*	Uses N watts for every M units of damage.
*	Heat/Cool
*	Uses N watts for every second in hot/cold area.
*	Long Jump
*	Uses N watts for every jump.
*	Alien Cloak
*	Uses N watts for each use. Each use lasts M seconds.
*	Alien Shield
*	Augments armor. Reduces Armor drain by one half
*/
class CItemSuit : public CItem
{
public:
	DECLARE_CLASS( CItemSuit, CItem );

	void Spawn( void ) override;
	void Precache( void ) override;
	bool MyTouch( CBasePlayer *pPlayer ) override;
};

#endif //GAME_SERVER_ENTITIES_ITEMS_CITEMSUIT_H