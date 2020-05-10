#ifndef GAME_SHARED_HUDCOLORS_H
#define GAME_SHARED_HUDCOLORS_H

#include "Color.h"

#define HUD_DEFAULT_PRIMARY_COLOR RGB_YELLOWISH
#define HUD_DEFAULT_EMPTYITEM_COLOR RGB_REDISH
#define HUD_DEFAULT_AMMOBAR_COLOR RGB_GREENISH

/**
*	Contains all customizable Hud colors.
*/
struct CHudColors final
{
public:
	Color m_PrimaryColor;
	Color m_EmptyItemColor;
	Color m_AmmoBarColor;

	CHudColors()
	{
		Reset();
	}

	void Reset()
	{
		m_PrimaryColor = HUD_DEFAULT_PRIMARY_COLOR;
		m_EmptyItemColor = HUD_DEFAULT_EMPTYITEM_COLOR;
		m_AmmoBarColor = HUD_DEFAULT_AMMOBAR_COLOR;
	}
};

#endif //GAME_SHARED_HUDCOLORS_H
