#ifndef GAME_SHARED_ENTITIES_WEAPONS_CAMMOTYPES_H
#define GAME_SHARED_ENTITIES_WEAPONS_CAMMOTYPES_H

/**
*	@file
*	Ammo type definitions.
*
*	- Solokiller
*/

#include <unordered_map>
#include <vector>

#include "StringUtils.h"

class CBasePlayer;

/**
*	Ammo ID type.
*/
typedef unsigned int AmmoID_t;

/**
*	Represents a single ammo type.
*/
class CAmmoType final
{
public:
	/**
	*	Constructor.
	*	@param pszName Name of this ammo type. Must point to memory that remains allocated as long as the map hasn't ended.
	*	@param ID Ammo ID. Must be unique.
	*	@param iMaxCarry Maximum amount of ammo of this type that a player can carry.
	*/
	CAmmoType( const char* const pszName, const AmmoID_t ID, const int iMaxCarry );

	/**
	*	@return The name of this ammo type.
	*/
	const char* GetName() const { return m_pszName; }

	/**
	*	@return The ID of this ammo type.
	*/
	AmmoID_t GetID() const { return m_ID; }

	/**
	*	@return The maximum amount of ammo of this type that a player can carry.
	*/
	int GetMaxCarry() const { return m_iMaxCarry; }

private:
	const char* const m_pszName;
	const AmmoID_t m_ID;
	const int m_iMaxCarry;

private:
	CAmmoType( const CAmmoType& ) = delete;
	CAmmoType& operator=( const CAmmoType& ) = delete;
};

/**
*	Represents a set of ammo types.
*/
class CAmmoTypes
{
public:
	/**
	*	Invalid ammo ID. Uses 0 to optimize network usage.
	*/
	static const AmmoID_t INVALID_AMMO_ID = 0;

	/**
	*	Number of bits used to store an ammo ID.
	*/
	static const size_t NUM_AMMO_ID_BITS = 8;

	/**
	*	The first ID that will be assigned to an ammo type.
	*/
	static const AmmoID_t FIRST_VALID_ID = 1;

	/**
	*	The last ID that will be assigned to an ammo type.
	*/
	static const AmmoID_t LAST_VALID_ID = ( 1 << NUM_AMMO_ID_BITS ) - 1;

	static const int MAX_AMMO_TYPES = LAST_VALID_ID + 1;

private:
	//Case insensitive hashing and comparison here. The original code uses stricmp.
	typedef std::unordered_map<const char*, size_t, RawCharHashI, RawCharEqualToI> AmmoMap_t;
	typedef std::vector<CAmmoType*> AmmoList_t;

public:
	/**
	*	Constructor.
	*/
	CAmmoTypes();

	/**
	*	Destructor.
	*/
	~CAmmoTypes();

	/**
	*	@return true if no ammo types have been registered.
	*/
	bool IsEmpty() const;

	/**
	*	@return The number of ammo types.
	*/
	size_t GetAmmoTypesCount() const;

	/**
	*	@return The first ammo ID that is in use. Returns INVALID_AMMO_ID if no ammo types have been added.
	*/
	AmmoID_t GetFirstAmmoID() const;

	/**
	*	@return The last ammo ID that is in use. Returns INVALID_AMMO_ID if no ammo types have been added.
	*/
	AmmoID_t GetLastAmmoID() const;

	/**
	*	Gets the ammo type at the given index.
	*	@param uiIndex Ammo index.
	*	@return The ammo type at the given index.
	*/
	const CAmmoType* GetAmmoTypeByIndex( const size_t uiIndex ) const;

	/**
	*	@copydoc GetAmmoTypeByIndex( const size_t uiIndex ) const
	*/
	CAmmoType* GetAmmoTypeByIndex( const size_t uiIndex );

	/**
	*	Gets the ammo type that has the given name.
	*	@param pszName Ammo name.
	*	@return The ammo type that has the given name.
	*/
	const CAmmoType* GetAmmoTypeByName( const char* const pszName ) const;

	/**
	*	@copydoc GetAmmoTypeByName( const char* const pszName ) const
	*/
	CAmmoType* GetAmmoTypeByName( const char* const pszName );

	/**
	*	Gets the ammo type with the given ID.
	*	@param ID Ammo ID.
	*	@return The ammo type that has the given ID.
	*/
	const CAmmoType* GetAmmoTypeByID( const AmmoID_t ID ) const;

	/**
	*	@copydoc GetAmmoTypeByID( const AmmoID_t ID ) const
	*/
	CAmmoType* GetAmmoTypeByID( const AmmoID_t ID );

	/**
	*	Pass in a name and this function will tell
	*	you the maximum amount of that type of ammunition that a 
	*	player can carry.
	*	@param pszName Ammo type name.
	*	@return Ammo type max carry value, or -1 if the given name is invalid.
	*/
	int GetMaxCarryByName( const char* const pszName ) const;

	/**
	*	Gets the ID of an ammo type, or INVALID_AMMO_ID if no such ammo type exists.
	*	@param pszName Ammo type name.
	*/
	AmmoID_t GetAmmoID( const char* const pszName ) const;

	/**
	*	@return Whether it is allowed to add new ammo types at this time.
	*/
	bool CanAddAmmoTypes() const { return m_bCanAddAmmoTypes; }

	/**
	*	Sets whether it is allowed to add new ammo types.
	*/
	void SetCanAddAmmoTypes( const bool bCanAddAmmoTypes )
	{
		m_bCanAddAmmoTypes = bCanAddAmmoTypes;
	}

	/**
	*	Adds a new ammo type.
	*	@param pszName Name of the ammo type. Must point to memory that remains allocated as long as the map hasn't ended.
	*	@param iMaxCarry Maximum amount of ammo of this type that a player can carry.
	*	@return If the type was successfully added, the object that represents the ammo type. If the ammo type already existed, returns the object for that type. Otherwise, returns null.
	*/
	CAmmoType* AddAmmoType( const char* const pszName, const int iMaxCarry );

	/**
	*	Removes all ammo types.
	*/
	void Clear();

	/**
	*	Generates a hash of all ammo types.
	*/
	size_t GenerateHash() const;

private:
	/*
	*	Maps a name to an index.
	*/
	AmmoMap_t m_AmmoMap;

	/*
	*	List of ammo types.
	*/
	AmmoList_t m_AmmoList;

	AmmoID_t m_NextID = FIRST_VALID_ID;

	bool m_bCanAddAmmoTypes = false;

private:
	CAmmoTypes( const CAmmoTypes& ) = delete;
	CAmmoTypes& operator=( const CAmmoTypes& ) = delete;
};

/**
*	Container class for ammo types used by weapons.
*/
extern CAmmoTypes g_AmmoTypes;

#endif //GAME_SHARED_ENTITIES_WEAPONS_CAMMOTYPES_H