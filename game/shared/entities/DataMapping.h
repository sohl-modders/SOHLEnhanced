#ifndef GAME_SHARED_ENTITIES_DATAMAPPING_H
#define GAME_SHARED_ENTITIES_DATAMAPPING_H

#include "CBaseForward.h"
#include "saverestore/SaveRestoreDefs.h"

struct TYPEDESCRIPTION;

#define DECLARE_CLASS_NOBASE( thisClass )	\
typedef thisClass ThisClass

#define DECLARE_CLASS( thisClass, baseClass )	\
DECLARE_CLASS_NOBASE( thisClass );				\
typedef baseClass BaseClass

/**
*	Represents the datamap for an entity.
*/
struct DataMap_t final
{
	/**
	*	Name of the class this belongs to.
	*/
	const char* pszClassName;

	/**
	*	Pointer to parent map, or null if no parent exists.
	*/
	const DataMap_t* pParent;

	/**
	*	Pointer to this entity's type description array.
	*/
	const TYPEDESCRIPTION* pTypeDesc;

	/**
	*	Number of descriptors in the type description array.
	*/
	size_t uiNumDescriptors;
};

/**
*	Specialized for each entity class that implements a datamap.
*	@tparam Entity class.
*	@return Whether the map was initialized or not.
*/
template<typename T>
bool InitDataMap()
{
	return false;
}

#define __DECLARE_DATADESC_NOBASE()				\
private:										\
	static DataMap_t m_DataMap;					\
												\
	template<typename T>						\
	friend bool InitDataMap();					\
												\
public:											\
	static const DataMap_t* GetBaseDataMap();	\
	static const DataMap_t* GetThisDataMap()

/**
*	Data descriptor for classes that have no vtable.
*/
#define DECLARE_DATADESC_FINAL()				\
__DECLARE_DATADESC_NOBASE();					\
	const DataMap_t* GetDataMap() const

//This trickery is needed to provide an override specifier for OSX compilation.
//It requires the override specifier for these methods, but we can't pass empty macro arguments.
//So we pass a macro name that expands to either override or nothing.
#define _DUMMY_MACRO()
#define _OVERRIDE_SPECIFIER() override

#define _DECLARE_DATADESC_NOBASE( overrideSpecifier )				\
__DECLARE_DATADESC_NOBASE();										\
	virtual const DataMap_t* GetDataMap() const overrideSpecifier()

/**
*	Data descriptor for the root class, if it has a vtable.
*/
#define DECLARE_DATADESC_NOBASE()				\
_DECLARE_DATADESC_NOBASE( _DUMMY_MACRO )

/**
*	Data descriptor for subclasses, if they have vtables.
*/
#define DECLARE_DATADESC()						\
_DECLARE_DATADESC_NOBASE( _OVERRIDE_SPECIFIER )

#define __BEGIN_DATADESC( thisClass )					\
														\
DataMap_t thisClass::m_DataMap;							\
														\
const DataMap_t* thisClass::GetThisDataMap()			\
{														\
	return &m_DataMap;									\
}														\
														\
const DataMap_t* thisClass::GetDataMap() const			\
{														\
	return &m_DataMap;									\
}														\
														\
template<>												\
bool InitDataMap<thisClass>();							\
														\
namespace __##thisClass##__Init							\
{														\
	const bool bInit = InitDataMap<thisClass>();		\
}														\
														\
template<>												\
bool InitDataMap<thisClass>()							\
{														\
	typedef thisClass ThisClass;						\
														\
	const char* const pszClassName = #thisClass;		\
														\
	static CMethodNameList methodNames( pszClassName );	\
														\
	static TYPEDESCRIPTION typeDesc[] =					\
	{

/**
*	Begins the data descriptor for the base class.
*/
#define BEGIN_DATADESC_NOBASE( thisClass )			\
const DataMap_t* thisClass::GetBaseDataMap()		\
{													\
	return nullptr;									\
}													\
													\
__BEGIN_DATADESC( thisClass )

/**
*	Begins the data descriptor for subclasses.
*/
#define BEGIN_DATADESC( thisClass )					\
const DataMap_t* thisClass::GetBaseDataMap()		\
{													\
	return thisClass::BaseClass::GetThisDataMap();	\
}													\
													\
__BEGIN_DATADESC( thisClass )

/**
*	Ends the data descriptor.
*/
#define END_DATADESC()											\
		{ FIELD_CHARACTER, "Dummy", nullptr, 0, 0, 0, nullptr }	\
	};															\
																\
	DataMap_t* pDataMap = &ThisClass::m_DataMap;				\
	pDataMap->pszClassName = pszClassName;						\
	pDataMap->pParent = ThisClass::GetBaseDataMap();			\
	pDataMap->pTypeDesc = typeDesc;								\
	pDataMap->uiNumDescriptors = ARRAYSIZE( typeDesc ) - 1;		\
																\
	return true;												\
}

/**
*	Finds a type description entry in a single data map.
*	@param dataMap Data map to search in.
*	@param pszFieldName Name of the field to search for.
*	@param bComparePublicName Whether the compare the public name or the field name.
*	@return If found, returns the type description. Otherwise, returns nullptr.
*/
const TYPEDESCRIPTION* UTIL_FindTypeDescInSingleDataMap( const DataMap_t& dataMap, const char* const pszFieldName, const bool bComparePublicName = false );

/**
*	Finds a type description entry in a data map. Will search in all parent data maps until found.
*	@param dataMap Data map to search in.
*	@param pszFieldName Name of the field to search for.
*	@param bComparePublicName Whether the compare the public name or the field name.
*	@return If found, returns the type description. Otherwise, returns nullptr.
*/
const TYPEDESCRIPTION* UTIL_FindTypeDescInDataMap( const DataMap_t& dataMap, const char* const pszFieldName, const bool bComparePublicName = false );

/**
*	Gets the name of a function out of a single data map from an address.
*	@param dataMap Data map to search in.
*	@param pFunction Function to search for.
*	@return If found, the name of the function. Otherwise, nullptr.
*/
const char* UTIL_NameFromFunctionSingle( const DataMap_t& dataMap, BASEPTR pFunction );

/**
*	Gets the name of a function out of a data map from an address. Searches in all parent data maps.
*	@param dataMap Data map to search in.
*	@param pFunction Function to search for.
*	@return If found, the name of the function. Otherwise, nullptr.
*/
const char* UTIL_NameFromFunction( const DataMap_t& dataMap, BASEPTR pFunction );

template<typename FUNCPTR>
const char* UTIL_NameFromFunction( const DataMap_t& dataMap, FUNCPTR pFunction )
{
	return UTIL_NameFromFunction( dataMap, reinterpret_cast<BASEPTR>( pFunction ) );
}

/**
*	Gets a function address out of a data map from a name. Searches in all parent data maps.
*	@param dataMap Data map to search in.
*	@param pszName Name of the function. Format: ClassNameFunctionName.
*	@return If found, the address of the function. Otherwise, nullptr.
*/
BASEPTR UTIL_FunctionFromName( const DataMap_t& dataMap, const char* const pszName );

#endif //GAME_SHARED_ENTITIES_DATAMAPPING_H
