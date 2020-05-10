#ifndef GAME_SERVER_ENTITIES_CPLAYERHUDCOLORS_H
#define GAME_SERVER_ENTITIES_CPLAYERHUDCOLORS_H

#include "Color.h"
#include "HudColors.h"

#include "CPointEntity.h"

enum
{
	/**
	*	Affect all players. If not set, affect activator.
	*/
	SF_PLR_HUDCOLORS_ALLPLAYERS		= 1 << 0,

	/**
	*	Reset player's hud colors to global setting.
	*/
	SF_PLR_HUDCOLORS_RESET			= 1 << 1,
};

/**
*	player_hudcolors
*
*	Changes the player's hud colors at will.
*/
class CPlayerHudColors : public CPointEntity
{
public:
	DECLARE_CLASS( CPlayerHudColors, CPointEntity );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData* pkvd ) override;

	void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue ) override;

private:
	CHudColors m_HudColors;
};

#endif //GAME_SERVER_ENTITIES_CPLAYERHUDCOLORS_H