#ifndef GAME_SHARED_ENTITIES_CENTITYDICTIONARY_H
#define GAME_SHARED_ENTITIES_CENTITYDICTIONARY_H

#include <unordered_map>

#include "StringUtils.h"

class CBaseEntityRegistry;
class CBaseEntity;

/**
*	Dictionary of entity classes.
*	Could potentially be used to register custom entity classes as well.
*/
class CEntityDictionary final
{
public:
	/**
	*	Callback used by EnumEntityClasses.
	*	@param reg Entity registry instance.
	*	@return true to continue, false to stop.
	*/
	using EntityEnumCallback = bool ( * )( CBaseEntityRegistry& reg );

private:
	typedef std::unordered_map<const char*, CBaseEntityRegistry*, RawCharHash, RawCharEqualTo> Entities_t;

public:
	CEntityDictionary() = default;
	~CEntityDictionary() = default;

	/**
	*	@return The number of classes.
	*/
	size_t GetNumClasses() const { return m_Entities.size(); }

	/**
	*	Finds an entity class by name.
	*	@param pszEntityName Name of the entity class to find.
	*	@return If found, the entity class, otherwise returns null.
	*/
	CBaseEntityRegistry* FindEntityClassByEntityName( const char* const pszEntityName );

	/**
	*	Adds a new entity class.
	*	@param pEntity Entity class to add.
	*	@return true if the class was added, false if it was already present or couldn't be added.
	*/
	bool AddEntityClass( CBaseEntityRegistry* pEntity );

	/**
	*	Creates a new instance of the entity, using the given entvars_t instance.
	*	@param pszEntityName Name of the entity to create.
	*	@param pev Entvars instance that will be assigned to the entity.
	*	@return Entity instance.
	*/
	CBaseEntity* CreateInstance( const char* const pszEntityName, entvars_t* pev );

	/**
	*	Creates a new instance of the entity, using the given edict_t instance.
	*	@param pszEntityName Name of the entity to create.
	*	@param pEdict Edict instance that will be assigned to the entity.
	*	@return Entity instance.
	*/
	CBaseEntity* CreateInstance( const char* const pszEntityName, edict_t* pEdict );

	/**
	*	Creates a new instance of the entity. Allocates a new edict.
	*	@param pszEntityName Name of the entity to create.
	*	@return Entity instance.
	*/
	CBaseEntity* CreateInstance( const char* const pszEntityName );

	/**
	*	Enumerates all entity classes, invoking pCallback on each class. The order of the classes is undefined.
	*/
	void EnumEntityClasses( EntityEnumCallback pCallback );

private:
	Entities_t m_Entities;

private:
	CEntityDictionary( const CEntityDictionary& ) = delete;
	CEntityDictionary& operator=( const CEntityDictionary& ) = delete;
};

CEntityDictionary& GetEntityDict();

#endif //GAME_SHARED_ENTITIES_CENTITYDICTIONARY_H