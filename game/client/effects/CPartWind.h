#ifndef GAME_CLIENT_EFFECTS_CPARTWIND_H
#define GAME_CLIENT_EFFECTS_CPARTWIND_H

#include "particleman.h"

class CPartWind : public CBaseParticle
{
public:
	CPartWind() = default;

	void Think( float flTime ) override;
};

#endif //GAME_CLIENT_EFFECTS_CPARTWIND_H