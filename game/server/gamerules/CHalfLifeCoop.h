#ifndef GAME_SERVER_GAMERULES_CHALFLIFECOOP_H
#define GAME_SERVER_GAMERULES_CHALFLIFECOOP_H

#include "CHalfLifeMultiplay.h"
#include "CHalfLifeRules.h"

/**
*	Rules for the basic half life co-op competition. - Solokiller
*/
template<typename BASERULES>
class CBaseHalfLifeCoop : public BASERULES
{
public:
	DECLARE_CLASS( CBaseHalfLifeCoop, BASERULES );

	bool IsMultiplayer() const override { return true; }

	bool IsDeathmatch() const override { return false; }

	bool IsCoOp() const override { return true; }

	bool FPlayerCanTakeDamage( CBasePlayer *pPlayer, const CTakeDamageInfo& info ) override;

	//Co-op without monsters would be rather boring.
	bool FAllowMonsters() const override { return true; }

	int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget ) override;
};

template<typename BASERULES>
inline bool CBaseHalfLifeCoop<BASERULES>::FPlayerCanTakeDamage( CBasePlayer *pPlayer, const CTakeDamageInfo& info )
{
	if( info.GetAttacker() && PlayerRelationship( pPlayer, info.GetAttacker() ) == GR_TEAMMATE )
	{
		// my teammate hit me.
		//TODO: should co-op have friendly fire? - Solokiller
		if( ( friendlyfire.value == 0 ) && ( info.GetAttacker() != pPlayer ) )
		{
			// friendly fire is off, and this hit came from someone other than myself,  then don't get hurt
			return false;
		}
	}

	return BaseClass::FPlayerCanTakeDamage( pPlayer, info );
}

template<typename BASERULES>
inline int CBaseHalfLifeCoop<BASERULES>::PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget )
{
	if( !pTarget || !pTarget->IsPlayer() )
		return GR_NOTTEAMMATE;

	return GR_TEAMMATE;
}

typedef CBaseHalfLifeCoop<CHalfLifeRules> CHalfLifeSingleCoop;
typedef CBaseHalfLifeCoop<CHalfLifeMultiplay> CHalfLifeMultiCoop;

#endif //GAME_SERVER_GAMERULES_CHALFLIFECOOP_H
