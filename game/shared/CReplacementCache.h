#ifndef GAME_SHARED_CREPLACEMENTCACHE_H
#define GAME_SHARED_CREPLACEMENTCACHE_H

#include <memory>
#include <unordered_map>

#include "StringUtils.h"

class CReplacementMap;

/**
*	A cache of replacement maps. - Solokiller
*/
class CReplacementCache final
{
private:
	using Cache_t = std::unordered_map<std::string, std::unique_ptr<CReplacementMap>, CStdStringHashI, CStdStringEqualToI>;

public:
	CReplacementCache() = default;
	~CReplacementCache() = default;

	/**
	*	@return The number of the replacement maps that have been loaded.
	*/
	size_t GetNumMaps() const { return m_Cache.size(); }

	/**
	*	Gets a replacement map by name. If no map by that name has been loaded, returns null.
	*/
	CReplacementMap* GetMap( const char* const pszFileName ) const;

	/**
	*	Acquires a replacement map that represents the given replacement file.
	*	@param pszFileName Name of the file that contains replacement data.
	*	@return If the file was successfully loaded, a pointer to the replacement map. Otherwise, null.
	*/
	CReplacementMap* AcquireMap( const char* const pszFileName );

private:
	std::unique_ptr<CReplacementMap> LoadMap( const char* const pszFileName, const char* const pszAbsFileName ) const;

private:
	Cache_t m_Cache;

private:
	CReplacementCache( const CReplacementCache& ) = delete;
	CReplacementCache& operator=( const CReplacementCache& ) = delete;
};

#endif //GAME_SHARED_CREPLACEMENTCACHE_H
