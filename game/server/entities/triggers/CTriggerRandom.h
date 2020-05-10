#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERRANDOM_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERRANDOM_H

#include <climits>

class CTriggerRandom : public CBaseDelay
{
private:
	static const size_t MAX_TARGETS = 16;

	static const size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

	/**
	*	Never trigger the same target more than once.
	*/
	static const int SF_UNIQUE			= 1 << 0;

	/**
	*	Trigger targets automatically when active.
	*/
	static const int SF_TIMED			= 1 << 1;

	/**
	*	If SF_UNIQUE is set, once all targets have been triggered, resets self to trigger targets again.
	*/
	static const int SF_REUSABLE		= 1 << 2;

	/**
	*	If SF_TIMED is set, start on when spawned.
	*/
	static const int SF_START_ON		= 1 << 3;

	/**
	*	If SF_TIMED is set, trigger one target and turn self off.
	*/
	static const int SF_TRIGGER_ONCE	= 1 << 4;

public:
	DECLARE_CLASS( CTriggerRandom, CBaseDelay );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData* pkvd ) override;

	void Spawn() override;

	inline bool IsUnique() const { return GetSpawnFlags().Any( SF_UNIQUE ); }

	inline bool IsTimed() const { return GetSpawnFlags().Any( SF_TIMED ); }

	inline bool IsReusable() const { return GetSpawnFlags().Any( SF_REUSABLE ); }

	inline bool ShouldStartOn() const { return GetSpawnFlags().Any( SF_START_ON ); }

	inline bool ShouldTriggerOnce() const { return GetSpawnFlags().Any( SF_TRIGGER_ONCE ); }

	/**
	*	Calculates the next time delay for timed random behavior.
	*/
	inline float NextTimeDelay() const
	{
		if( m_flMinTime == 0 )
			return m_flMaxTime;

		if( m_flMaxTime == 0 )
			return m_flMinTime;

		return UTIL_RandomFloat( m_flMinTime, m_flMaxTime );
	}

	/**
	*	Use for regular (non-timed) random firing behavior.
	*/
	void RandomUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue );

	/**
	*	Use to toggle timed thinking on and off.
	*/
	void ToggleUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue );

	/**
	*	Think that fires a target every time it's ran.
	*/
	void RandomThink();

private:
	/**
	*	Selects a target to fire next.
	*	@param bUnique Whether to check if targets have already been fired.
	*	@return If a target was found, returns the index of that target. Otherwise, returns INVALID_INDEX.
	*/
	size_t SelectTarget( const bool bUnique ) const;

	/**
	*	Fires a random target.
	*	@param pActivator Activator to pass to targets.
	*	@param bUnique Whether to check if targets have already been fired.
	*	@return Whether a target was fired.
	*/
	bool FireRandomTarget( CBaseEntity* pActivator, const bool bUnique );

private:
	int m_iTargetCount = 0;
	int m_iTargetsFiredCount = 0;

	float m_flMinTime = 0;
	float m_flMaxTime = 0;

	string_t m_iszTargets[ MAX_TARGETS ] = {};

	bool m_bFiredTargets[ MAX_TARGETS ] = {};
};

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERRANDOM_H
