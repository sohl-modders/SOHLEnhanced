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
#ifndef GAME_SERVER_ENTITIES_NPCS_SCRIPTED_CSCRIPTEDSENTENCE_H
#define GAME_SERVER_ENTITIES_NPCS_SCRIPTED_CSCRIPTEDSENTENCE_H

#define SF_SENTENCE_ONCE		0x0001
#define SF_SENTENCE_FOLLOWERS	0x0002	// only say if following player
#define SF_SENTENCE_INTERRUPT	0x0004	// force talking except when dead
#define SF_SENTENCE_CONCURRENT	0x0008	// allow other people to keep talking

class CScriptedSentence : public CBaseToggle
{
public:
	enum class SoundRadius
	{
		SMALL = 0,
		MEDIUM,
		LARGE,
		PLAY_EVERYWHERE
	};

public:
	DECLARE_CLASS( CScriptedSentence, CBaseToggle );
	DECLARE_DATADESC();

	void Spawn() override;
	void KeyValue( KeyValueData *pkvd ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void FindThink();
	void DelayThink();
	int	 ObjectCaps() const override { return ( CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION ); }

	CBaseMonster *FindEntity();
	bool AcceptableSpeaker( const CBaseMonster *pMonster ) const;
	bool StartSentence( CBaseMonster *pTarget );


private:
	int		m_iszSentence;		// string index for idle animation
	int		m_iszEntity;	// entity that is wanted for this sentence
	float	m_flRadius;		// range to search
	float	m_flDuration;	// How long the sentence lasts
	float	m_flRepeat;	// repeat rate
	float	m_flAttenuation;
	float	m_flVolume;
	bool	m_active;
	int		m_iszListener;	// name of entity to look at while talking
};

#endif //GAME_SERVER_ENTITIES_NPCS_SCRIPTED_CSCRIPTEDSENTENCE_H