#ifndef GAME_CLIENT_EFFECTS_CPARTRAINDROP_H
#define GAME_CLIENT_EFFECTS_CPARTRAINDROP_H

#include "particleman.h"

class CPartRainDrop : public CBaseParticle
{
public:
	CPartRainDrop() = default;

	void Think( float flTime ) override;

	void Touch( Vector pos, Vector normal, int index ) override;

private:
	bool m_bTouched = false;
};

#endif //GAME_CLIENT_EFFECTS_CPARTRAINDROP_H