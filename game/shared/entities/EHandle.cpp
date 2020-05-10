#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "EHandle.h"

EHANDLE::EHANDLE( CBaseEntity* pEntity )
	: m_pent( nullptr )
	, m_serialnumber( 0 )
{
	*this = pEntity;
}

EHANDLE::EHANDLE( const EHANDLE& other )
	: m_pent( other.m_pent )
	, m_serialnumber( other.m_serialnumber )
{
}

EHANDLE::EHANDLE( edict_t* pEdict, int iSerialNumber )
	: m_pent( pEdict )
	, m_serialnumber( iSerialNumber )
{
}

edict_t * EHANDLE::Get() const
{
	if( m_pent )
	{
		if( m_pent->serialnumber == m_serialnumber )
			return m_pent;
		else
			return nullptr;
	}
	return nullptr;
}

edict_t * EHANDLE::Set( edict_t *pent )
{
	m_pent = pent;
	if( pent )
		m_serialnumber = m_pent->serialnumber;
	return pent;
}


EHANDLE :: operator CBaseEntity *( )
{
	return ( CBaseEntity * ) GET_PRIVATE( Get() );
}

EHANDLE::operator const CBaseEntity*( ) const
{
	return ( CBaseEntity * ) GET_PRIVATE( Get() );
}

CBaseEntity * EHANDLE :: operator = ( CBaseEntity *pEntity )
{
	if( pEntity )
	{
		m_pent = ENT( pEntity->pev );
		if( m_pent )
			m_serialnumber = m_pent->serialnumber;
	}
	else
	{
		m_pent = NULL;
		m_serialnumber = 0;
	}
	return pEntity;
}

CBaseEntity * EHANDLE :: operator -> () const
{
	return ( CBaseEntity * ) GET_PRIVATE( Get() );
}