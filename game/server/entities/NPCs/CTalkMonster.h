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
#ifndef TALKMONSTER_H
#define TALKMONSTER_H

#ifndef MONSTERS_H
#include "entities/NPCs/Monsters.h"
#endif

//=========================================================
// Talking monster base class
// Used for scientists and barneys
//=========================================================

#define TALKRANGE_MIN 500.0				// don't talk to anyone farther away than this

#define TLK_STARE_DIST	128				// anyone closer than this and looking at me is probably staring at me.

#define bit_saidDamageLight		(1<<0)	// bits so we don't repeat key sentences
#define bit_saidDamageMedium	(1<<1)
#define bit_saidDamageHeavy		(1<<2)
#define bit_saidHelloPlayer		(1<<3)
#define bit_saidWoundLight		(1<<4)
#define bit_saidWoundHeavy		(1<<5)
#define bit_saidHeard			(1<<6)
#define bit_saidSmelled			(1<<7)

#define TLK_CFRIENDS		3

enum TALKGROUPNAMES
{
	TLK_ANSWER = 0,
	TLK_QUESTION,
	TLK_IDLE,
	TLK_STARE,
	TLK_USE,
	TLK_UNUSE,
	TLK_STOP,
	TLK_NOSHOOT,
	TLK_HELLO,
	TLK_PHELLO,
	TLK_PIDLE,
	TLK_PQUESTION,
	TLK_PLHURT1,
	TLK_PLHURT2,
	TLK_PLHURT3,
	TLK_SMELL,
	TLK_WOUND,
	TLK_MORTAL,

	TLK_CGROUPS,					// MUST be last entry
};


enum
{
	SCHED_CANT_FOLLOW = LAST_COMMON_SCHEDULE + 1,
	SCHED_MOVE_AWAY,		// Try to get out of the player's way
	SCHED_MOVE_AWAY_FOLLOW,	// same, but follow afterward
	SCHED_MOVE_AWAY_FAIL,	// Turn back toward player

	LAST_TALKMONSTER_SCHEDULE,		// MUST be last
};

enum
{
	TASK_CANT_FOLLOW = LAST_COMMON_TASK + 1,
	TASK_MOVE_AWAY_PATH,
	TASK_WALK_PATH_FOR_UNITS,

	TASK_TLK_RESPOND,		// say my response
	TASK_TLK_SPEAK,			// question or remark
	TASK_TLK_HELLO,			// Try to say hello to player
	TASK_TLK_HEADRESET,		// reset head position
	TASK_TLK_STOPSHOOTING,	// tell player to stop shooting friend
	TASK_TLK_STARE,			// let the player know I know he's staring at me.
	TASK_TLK_LOOK_AT_CLIENT,// faces player if not moving and not talking and in idle.
	TASK_TLK_CLIENT_STARE,	// same as look at client, but says something if the player stares.
	TASK_TLK_EYECONTACT,	// maintain eyecontact with person who I'm talking to
	TASK_TLK_IDEALYAW,		// set ideal yaw to face who I'm talking to
	TASK_FACE_PLAYER,		// Face the player

	LAST_TALKMONSTER_TASK,			// MUST be last
};

class CTalkMonster : public CBaseMonster
{
public:
	DECLARE_CLASS( CTalkMonster, CBaseMonster );
	DECLARE_DATADESC();

	void			TalkInit( void );				
	CBaseEntity*	FindNearestFriend( const bool fPlayer ) const;
	float			TargetDistance( void );
	void			StopTalking( void ) { SentenceStop(); }
	
	// Base Monster functions
	void			Precache( void ) override;
	void			OnTakeDamage( const CTakeDamageInfo& info ) override;
	void			Touch(	CBaseEntity *pOther ) override;
	void			Killed( const CTakeDamageInfo& info, GibAction gibAction ) override;
	Relationship	IRelationship ( CBaseEntity *pTarget ) override;
	virtual bool	CanPlaySentence( const bool fDisregardState ) const override;
	virtual void	PlaySentence( const char *pszSentence, float duration, float volume, float attenuation ) override;
	void			PlayScriptedSentence( const char *pszSentence, float duration, float volume, float attenuation, const bool bConcurrent, CBaseEntity *pListener ) override;
	void			KeyValue( KeyValueData *pkvd ) override;

	// AI functions
	void			SetActivity ( Activity newActivity ) override;
	Schedule_t		*GetScheduleOfType ( int Type ) override;
	void			StartTask( const Task_t& task ) override;
	void			RunTask( const Task_t& task ) override;
	void			HandleAnimEvent( AnimEvent_t& event ) override;
	void			PrescheduleThink( void ) override;
	

	// Conversations / communication
	int				GetVoicePitch( void );
	void			IdleRespond( void );
	bool			FIdleSpeak();
	bool			FIdleStare();
	bool			FIdleHello();
	void			IdleHeadTurn( const Vector &vecFriend );
	bool			FOkToSpeak() const;
	void			TrySmellTalk( void );
	CBaseEntity		*EnumFriends( CBaseEntity *pentPrevious, int listNumber, const bool bTrace );
	void			AlertFriends( void );
	void			ShutUpFriends( void );
	bool			IsTalking() const;
	void			Talk( float flDuration );	
	// For following
	bool			CanFollow() const;
	bool			IsFollowing() const { return m_hTargetEnt != nullptr && m_hTargetEnt->IsPlayer(); }
	void			StopFollowing( const bool clearSchedule ) override;
	void			StartFollowing( CBaseEntity *pLeader );
	virtual void	DeclineFollowing( void ) {}
	void			LimitFollowers( CBaseEntity *pPlayer, int maxFollowers );

	void 	FollowerUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	
	virtual void	SetAnswerQuestion( CTalkMonster *pSpeaker );
	virtual int		FriendNumber( int arrayNumber ) const { return arrayNumber; }
	
	static const char* m_szFriends[TLK_CFRIENDS];		// array of friend names
	static float g_talkWaitTime;
	
	int			m_bitsSaid;						// set bits for sentences we don't want repeated
	int			m_nSpeak;						// number of times initiated talking
	int			m_voicePitch;					// pitch of voice for this head
	const char	*m_szGrp[TLK_CGROUPS];			// sentence group names
	float		m_useTime;						// Don't allow +USE until this time
	int			m_iszUse;						// Custom +USE sentence group (follow)
	int			m_iszUnUse;						// Custom +USE sentence group (stop following)

	float		m_flLastSaidSmelled;// last time we talked about something that stinks
	float		m_flStopTalkTime;// when in the future that I'll be done saying this sentence.

	EHANDLE		m_hTalkTarget;	// who to look at while talking
	DECLARE_SCHEDULES() override;
};


// Clients can push talkmonsters out of their way
#define		bits_COND_CLIENT_PUSH		( bits_COND_SPECIAL1 )
// Don't see a client right now.
#define		bits_COND_CLIENT_UNSEEN		( bits_COND_SPECIAL2 )


#endif		//TALKMONSTER_H