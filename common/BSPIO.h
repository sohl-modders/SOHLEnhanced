#ifndef COMMON_BSPIO_H
#define COMMON_BSPIO_H

namespace bsp
{
using ParseEntCallback = const char* ( *)( const char* pszBuffer, bool& bError );

/**
*	Open the .bsp and read in the entity lump
*/
char* LoadEntityLump( const char* const pszFileName );

/**
*	Frees the entity lump.
*/
inline void FreeEntityLump( char* pszBuffer )
{
	delete[] pszBuffer;
}

/**
*	Parse through entity lump looking for requested info.
*/
void ProcessEnts( const char* pszBuffer, ParseEntCallback pCallback );

/**
*	Evaluate Key/Value pairs for the entity
*/
const char* ParseKeyValue( const char* pszBuffer, char* pszKey, const size_t uiKeySize, char* pszValue, const size_t uiValueSize, bool& bError, bool& bEnd );
}

#endif //COMMON_BSPIO_H