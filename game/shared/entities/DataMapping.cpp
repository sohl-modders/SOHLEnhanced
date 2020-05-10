#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "DataMapping.h"

const TYPEDESCRIPTION* UTIL_FindTypeDescInSingleDataMap( const DataMap_t& dataMap, const char* const pszFieldName, const bool bComparePublicName )
{
	ASSERT( pszFieldName );

	const TYPEDESCRIPTION* pDesc;

	const char* pszName;

	for( size_t uiIndex = 0; uiIndex < dataMap.uiNumDescriptors; ++uiIndex )
	{
		pDesc = &dataMap.pTypeDesc[ uiIndex ];

		pszName = bComparePublicName ? pDesc->pszPublicName : pDesc->fieldName;

		if( pszName && stricmp( pszName, pszFieldName ) == 0 )
			return pDesc;
	}

	return nullptr;
}

const TYPEDESCRIPTION* UTIL_FindTypeDescInDataMap( const DataMap_t& dataMap, const char* const pszFieldName, const bool bComparePublicName )
{
	const DataMap_t* pMap = &dataMap;

	while( pMap )
	{
		if( auto pDesc = UTIL_FindTypeDescInSingleDataMap( *pMap, pszFieldName, bComparePublicName ) )
			return pDesc;

		pMap = pMap->pParent;
	}

	return nullptr;
}

const char* UTIL_NameFromFunctionSingle( const DataMap_t& dataMap, BASEPTR pFunction )
{
	ASSERT( pFunction );

	const TYPEDESCRIPTION* pDesc;

	for( size_t uiIndex = 0; uiIndex < dataMap.uiNumDescriptors; ++uiIndex )
	{
		pDesc = &dataMap.pTypeDesc[ uiIndex ];

		if( pDesc->pFunction == pFunction )
			return pDesc->pszPublicName;
	}

	return nullptr;
}

const char* UTIL_NameFromFunction( const DataMap_t& dataMap, BASEPTR pFunction )
{
	ASSERT( pFunction );

	const DataMap_t* pMap = &dataMap;

	while( pMap )
	{
		if( auto pszName = UTIL_NameFromFunctionSingle( *pMap, pFunction ) )
			return pszName;

		pMap = pMap->pParent;
	}

	return nullptr;
}

BASEPTR UTIL_FunctionFromName( const DataMap_t& dataMap, const char* const pszName )
{
	if( auto pDesc = UTIL_FindTypeDescInDataMap( dataMap, pszName, true ) )
		return pDesc->pFunction;

	return nullptr;
}
