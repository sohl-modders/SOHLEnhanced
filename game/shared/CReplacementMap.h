#ifndef GAME_SHARED_CREPLACEMENTMAP_H
#define GAME_SHARED_CREPLACEMENTMAP_H

#include <string>
#include <unordered_map>

#include "StringUtils.h"

/**
*	A map of filenames used for model/sound replacement. - Solokiller
*/
class CReplacementMap final
{
private:
	/*
	*	Case insensitive map.
	*/
	using ReplacementMap_t = std::unordered_map<std::string, std::string, CStdStringHashI, CStdStringEqualToI>;

public:
	CReplacementMap( const char* const pszFileName );
	~CReplacementMap() = default;

	/**
	*	@return The filename.
	*/
	const std::string& GetFileName() const { return m_szFileName; }

	/**
	*	@return The number of replacement entries.
	*/
	size_t GetNumEntries() const { return m_Map.size(); }

	/**
	*	Looks up a file and returns the file that should be used in its place.
	*	Returns pszFileName if there is no replacement.
	*	The returned string will not remain valid if the map is modified.
	*/
	const char* LookupFile( const char* const pszFileName ) const;

	/**
	*	Adds a replacement entry. If the original file is already in the map, does nothing and returns true.
	*	@param pszFileName Name of the file to replace.
	*	@param pszReplacement Name of the file to replace with.
	*	@return true if the entry was added, false otherwise.
	*/
	bool AddReplacement( const char* const pszFileName, const char* const pszReplacement );

private:
	std::string m_szFileName;
	ReplacementMap_t m_Map;

private:
	CReplacementMap( const CReplacementMap& ) = delete;
	CReplacementMap& operator=( const CReplacementMap& ) = delete;
};

#endif //GAME_SHARED_CREPLACEMENTMAP_H
