#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "entities/CBreakable.h"
#include "CGib.h"

#include "CEnvShooter.h"

LINK_ENTITY_TO_CLASS( env_shooter, CEnvShooter );

void CEnvShooter::Precache( void )
{
	m_iGibModelIndex = PRECACHE_MODEL( GetModelName() );
	CBreakable::MaterialSoundPrecache( ( Materials ) m_iGibMaterial );
}

void CEnvShooter::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "shootmodel" ) )
	{
		SetModelName( ALLOC_STRING( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "shootsounds" ) )
	{
		int iNoise = atoi( pkvd->szValue );
		pkvd->fHandled = true;
		switch( iNoise )
		{
		case 0:
			m_iGibMaterial = matGlass;
			break;
		case 1:
			m_iGibMaterial = matWood;
			break;
		case 2:
			m_iGibMaterial = matMetal;
			break;
		case 3:
			m_iGibMaterial = matFlesh;
			break;
		case 4:
			m_iGibMaterial = matRocks;
			break;

		default:
		case -1:
			m_iGibMaterial = matNone;
			break;
		}
	}
	else
	{
		CGibShooter::KeyValue( pkvd );
	}
}

CGib *CEnvShooter::CreateGib( void )
{
	auto pGib = CGib::GibCreate( GetModelName() );

	int bodyPart = 0;

	if( GetBody() > 1 )
		bodyPart = RANDOM_LONG( 0, GetBody() - 1 );

	pGib->SetBody( bodyPart );
	pGib->m_bloodColor = DONT_BLEED;
	pGib->m_material = m_iGibMaterial;

	pGib->SetRenderMode( GetRenderMode() );
	pGib->SetRenderAmount( GetRenderAmount() );
	pGib->SetRenderColor( GetRenderColor() );
	pGib->SetRenderFX( GetRenderFX() );
	pGib->SetScale( GetScale() );
	pGib->SetSkin( GetSkin() );

	return pGib;
}