#ifndef GAME_CLIENT_EFFECTS_CPARTGRASSPIECE_H
#define GAME_CLIENT_EFFECTS_CPARTGRASSPIECE_H

#include "particleman.h"

class CPartGrassPiece : public CBaseParticle
{
public:
	CPartGrassPiece() = default;

	void Think( float flTime ) override;
};

#endif //GAME_CLIENT_EFFECTS_CPARTGRASSPIECE_H