#include <cassert>

#include "CReplacementMap.h"

CReplacementMap::CReplacementMap( const char* const pszFileName )
	: m_szFileName( pszFileName )
{
	//Will probably never fire since std::string throws an exception if a null pointer is given (on Windows at least) - Solokiller
	assert( pszFileName );
}

const char* CReplacementMap::LookupFile( const char* const pszFileName ) const
{
	assert( pszFileName );

	auto it = m_Map.find( pszFileName );

	if( it == m_Map.end() )
		return pszFileName;

	return it->second.c_str();
}

bool CReplacementMap::AddReplacement( const char* const pszFileName, const char* const pszReplacement )
{
	assert( pszFileName );
	assert( pszReplacement );

	if( LookupFile( pszFileName ) != pszFileName )
		return true;

	auto result = m_Map.emplace( pszFileName, pszReplacement );

	return result.second;
}