#ifndef GAME_SHARED_DAMAGE_H
#define GAME_SHARED_DAMAGE_H

// used by suit voice to indicate damage sustained and repaired type to player

// instant damage

enum Damage
{
	DMG_GENERIC				= 0,			// generic damage was done
	DMG_CRUSH				= 1 << 0,		// crushed by falling or moving object
	DMG_BULLET				= 1 << 1,		// shot
	DMG_SLASH				= 1 << 2,		// cut, clawed, stabbed
	DMG_BURN				= 1 << 3,		// heat burned
	DMG_FREEZE				= 1 << 4,		// frozen
	DMG_FALL				= 1 << 5,		// fell too far
	DMG_BLAST				= 1 << 6,		// explosive blast damage
	DMG_CLUB				= 1 << 7,		// crowbar, punch, headbutt
	DMG_SHOCK				= 1 << 8,		// electric shock
	DMG_SONIC				= 1 << 9,		// sound pulse shockwave
	DMG_ENERGYBEAM			= 1 << 10,		// laser or other high energy beam 
	DMG_NEVERGIB			= 1 << 12,		// with this bit OR'd in, no damage type will be able to gib victims upon death
	DMG_ALWAYSGIB			= 1 << 13,		// with this bit OR'd in, any damage type can be made to gib victims upon death.
	DMG_DROWN				= 1 << 14,		// Drowning
// time-based damage
	DMG_TIMEBASED			= ~( 0x3fff ),	// mask for time-based damage

	DMG_PARALYZE			= 1 << 15,		// slows affected creature down
	DMG_NERVEGAS			= 1 << 16,		// nerve toxins, very bad
	DMG_POISON				= 1 << 17,		// blood poisioning
	DMG_RADIATION			= 1 << 18,		// radiation exposure
	DMG_DROWNRECOVER		= 1 << 19,		// drowning recovery
	DMG_ACID				= 1 << 20,		// toxic chemicals or acid burns
	DMG_SLOWBURN			= 1 << 21,		// in an oven
	DMG_SLOWFREEZE			= 1 << 22,		// in a subzero freezer
	DMG_MORTAR				= 1 << 23,		// Hit by air raid (done to distinguish grenade from mortar)

	// these are the damage types that are allowed to gib corpses
	DMG_GIB_CORPSE			= ( DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB ),

	// these are the damage types that have client hud art
	DMG_SHOWNHUD			= ( DMG_POISON | DMG_ACID | DMG_FREEZE | DMG_SLOWFREEZE | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK ),

	//TF ADDITIONS
	DMG_IGNITE				= 1 << 24,		// Players hit by this begin to burn
	DMG_RADIUS_MAX			= 1 << 25,		// Radius damage with this flag doesn't decrease over distance
	DMG_RADIUS_QUAKE		= 1 << 26,		// Radius damage is done like Quake. 1/2 damage at 1/2 radius.
	DMG_IGNOREARMOR			= 1 << 27,		// Damage ignores target's armor
	DMG_AIMED				= 1 << 28,		// Does Hit location damage
	DMG_WALLPIERCING		= 1 << 29,		// Blast Damages ents through walls

	DMG_CALTROP				= 1 << 30,
	DMG_HALLUC				= 1 << 31,

	// TF Healing Additions for TakeHealth
	DMG_IGNORE_MAXHEALTH	= DMG_IGNITE,

	// TF Redefines since we never use the originals
	DMG_NAIL				= DMG_SLASH,
	DMG_NOT_SELF			= DMG_FREEZE,
	
	DMG_TRANQ				= DMG_MORTAR,
	DMG_CONCUSS				= DMG_SONIC,
};

// NOTE: tweak these values based on gameplay feedback:

/**
*	Number of 2 second intervals to take damage.
*/
#define PARALYZE_DURATION	2

/**
*	Damage to take each 2 second interval.
*/
#define PARALYZE_DAMAGE		1.0

#define NERVEGAS_DURATION	2
#define NERVEGAS_DAMAGE		5.0

#define POISON_DURATION		5
#define POISON_DAMAGE		2.0

#define RADIATION_DURATION	2
#define RADIATION_DAMAGE	1.0

#define ACID_DURATION		2
#define ACID_DAMAGE			5.0

#define SLOWBURN_DURATION	2
#define SLOWBURN_DAMAGE		1.0

#define SLOWFREEZE_DURATION	2
#define SLOWFREEZE_DAMAGE	1.0

/**
*	Time based Damage works as follows:
*	1) There are several types of timebased damage:
*	
*	DMG_PARALYZE
*	DMG_NERVEGAS
*	DMG_POISON
*	DMG_RADIATION
*	DMG_DROWNRECOVER
*	DMG_ACID
*	DMG_SLOWBURN
*	DMG_SLOWFREEZE
*	
*	2) A new hit inflicting tbd restarts the tbd counter - each monster has an 8bit counter,
*	per damage type. The counter is decremented every second, so the maximum time
*	an effect will last is 255/60 = 4.25 minutes.  Of course, staying within the radius
*	of a damaging effect like fire, nervegas, radiation will continually reset the counter to max.
*	
*	3) Every second that a tbd counter is running, the player takes damage.  The damage
*	is determined by the type of tdb.
*	Paralyze		- 1/2 movement rate, 30 second duration.
*	Nervegas		- 5 points per second, 16 second duration = 80 points max dose.
*	Poison			- 2 points per second, 25 second duration = 50 points max dose.
*	Radiation		- 1 point per second, 50 second duration = 50 points max dose.
*	Drown			- 5 points per second, 2 second duration.
*	Acid/Chemical	- 5 points per second, 10 second duration = 50 points max.
*	Burn			- 10 points per second, 2 second duration.
*	Freeze			- 3 points per second, 10 second duration = 30 points max.
*	
*	4) Certain actions or countermeasures counteract the damaging effects of tbds:
*	
*	Armor/Heater/Cooler - Chemical(acid),burn, freeze all do damage to armor power, then to body
*	- recharged by suit recharger
*	Air In Lungs		- drowning damage is done to air in lungs first, then to body
*						- recharged by poking head out of water
*						- 10 seconds if swiming fast
*	Air In SCUBA		- drowning damage is done to air in tanks first, then to body
*						- 2 minutes in tanks. Need new tank once empty.
*	Radiation Syringe	- Each syringe full provides protection vs one radiation dosage
*	Antitoxin Syringe	- Each syringe full provides protection vs one poisoning (nervegas or poison).
*	Health kit			- Immediate stop to acid/chemical, fire or freeze damage.
*	Radiation Shower	- Immediate stop to radiation damage, acid/chemical or fire damage.
*
*	If player is taking time based damage, continue doing damage to player -
*	this simulates the effect of being poisoned, gassed, dosed with radiation etc -
*	anything that continues to do damage even after the initial contact stops.
*	Update all time based damage counters, and shut off any that are done.
*	
*	The m_bitsDamageType bit MUST be set if any damage is to be taken.
*	This routine will detect the initial on value of the m_bitsDamageType
*	and init the appropriate counter.  Only processes damage every second.
*
*	PARALYZE_DURATION
*	PARALYZE_DAMAGE
*	
*	NERVEGAS_DURATION
*	NERVEGAS_DAMAGE
*	
*	POISON_DURATION
*	POISON_DAMAGE
*	
*	RADIATION_DURATION
*	RADIATION_DAMAGE
*	
*	ACID_DURATION
*	ACID_DAMAGE
*	
*	SLOWBURN_DURATION
*	SLOWBURN_DAMAGE
*	
*	SLOWFREEZE_DURATION
*	SLOWFREEZE_DAMAGE
*
*	@see CBasePlayer::CheckTimeBasedDamage()
*/

#endif //GAME_SHARED_DAMAGE_H