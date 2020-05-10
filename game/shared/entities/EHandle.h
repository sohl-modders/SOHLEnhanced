#ifndef GAME_SHARED_EHANDLE_H
#define GAME_SHARED_EHANDLE_H

struct edict_t;

class CBaseEntity;

//
// EHANDLE. Safe way to point to CBaseEntities who may die between frames
//
class EHANDLE
{
private:
	edict_t *m_pent;
	int		m_serialnumber;

public:
	EHANDLE( CBaseEntity* pEntity = nullptr );
	EHANDLE( const EHANDLE& other );
	EHANDLE( edict_t* pEdict, int iSerialNumber );

	edict_t *Get() const;
	edict_t *Set( edict_t *pent );

	operator CBaseEntity *( );

	operator const CBaseEntity*( ) const;

	EHANDLE& operator=( const EHANDLE& other ) = default;

	CBaseEntity * operator = ( CBaseEntity *pEntity );
	CBaseEntity * operator ->() const;

	const CBaseEntity* GetEntity() const { return *this; }
	CBaseEntity* GetEntity() { return *this; }
};

/**
*	Helper function to cast from an EHANDLE to an entity class without having to manually cast to CBaseEntity first.
*/
template<typename T>
T EHANDLE_cast( EHANDLE& handle )
{
	return static_cast<T>( static_cast<CBaseEntity*>( handle ) );
}

#endif //GAME_SHARED_EHANDLE_H