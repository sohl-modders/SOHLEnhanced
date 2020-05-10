#include "hud.h"
#include "cl_util.h"

#include "CPartWind.h"

void CPartWind::Think( float flTime )
{
	if( m_flDieTime - flTime <= 3.0 )
	{
		if( m_flBrightness > 0.0 )
		{
			m_flBrightness -= ( flTime - m_flTimeCreated ) * 0.4;
		}

		if( m_flBrightness < 0.0 )
		{
			m_flBrightness = 0;
			flTime = m_flDieTime = gEngfuncs.GetClientTime();
		}
	}
	else
	{
		if( m_flBrightness < 105.0 )
		{
			m_flBrightness += ( flTime - m_flTimeCreated ) * 5.0 + 4.0;
		}
	}

	CBaseParticle::Think( flTime );
}