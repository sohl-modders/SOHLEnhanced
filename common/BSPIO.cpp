#include "extdll.h"
#include "util.h"

#include "MiniBSPFile.h"

#include "BSPIO.h"

namespace bsp
{
char* LoadEntityLump( const char* const pszFileName )
{
	FileHandle_t fp = g_pFileSystem->Open( pszFileName, "rb" );
	if( fp == FILESYSTEM_INVALID_HANDLE )
		return nullptr;

	dheader_t header;

	// Read in the .bsp header
	if( g_pFileSystem->Read( &header, sizeof( dheader_t ), fp ) != sizeof( dheader_t ) )
	{
		Con_Printf( "bsp::LoadEntityLump: Could not read BSP header for map [%s].\n", pszFileName );
		g_pFileSystem->Close( fp );
		return nullptr;
	}

	// Check the version
	{
		const int iBSPVersion = header.version;
		if( iBSPVersion != BSPVERSION_QUAKE && iBSPVersion != BSPVERSION )
		{
			g_pFileSystem->Close( fp );
			Con_Printf( "bsp::LoadEntityLump: Map [%s] has incorrect BSP version (%i should be %i).\n", pszFileName, iBSPVersion, BSPVERSION );
			return nullptr;
		}
	}

	// Get entity lump
	lump_t* curLump = &header.lumps[ LUMP_ENTITIES ];
	// and entity lump size
	const int size = curLump->filelen;

	// Jump to it
	g_pFileSystem->Seek( fp, curLump->fileofs, FILESYSTEM_SEEK_HEAD );

	// Allocate sufficient memmory
	char* pszBuffer = new( std::nothrow ) char[ size + 1 ];
	if( !pszBuffer )
	{
		g_pFileSystem->Close( fp );
		Con_Printf( "bsp::LoadEntityLump: Couldn't allocate %i bytes\n", size + 1 );
		return nullptr;
	}

	// Read in the entity lump
	g_pFileSystem->Read( pszBuffer, size, fp );

	// Terminate the string
	pszBuffer[ size ] = '\0';

	if( fp )
	{
		g_pFileSystem->Close( fp );
	}

	return pszBuffer;
}

void ProcessEnts( const char* pszBuffer, ParseEntCallback pCallback )
{
	ASSERT( pszBuffer );
	ASSERT( pCallback );

	char token[ 1024 ];
	bool bError = false;

	// parse entities from entity lump of .bsp file
	while( 1 )
	{
		// parse the opening brace	
		pszBuffer = COM_Parse( pszBuffer, token, sizeof( token ) );
		if( !pszBuffer )
			break;

		// Didn't find opening brace?
		if( token[ 0 ] != '{' )
		{
			Con_Printf( "bsp::ProcessEnts: found %s when expecting {\n", token );
			return;
		}

		// Fill in data
		pszBuffer = pCallback( pszBuffer, bError );

		// Check for errors and abort if any
		if( bError )
		{
			Con_Printf( "bsp::ProcessEnts: error parsing entities\n" );
			return;
		}
	}
}

const char* ParseKeyValue( const char* pszBuffer, char* pszKey, const size_t uiKeySize, char* pszValue, const size_t uiValueSize, bool& bError, bool& bEnd )
{
	char token[ 1024 ];

	// Parse key
	pszBuffer = COM_Parse( pszBuffer, token, sizeof( token ) );
	if( token[ 0 ] == '}' )
	{
		bEnd = true;
		return pszBuffer;
	}

	bEnd = false;

	// Ran out of input buffer?
	if( !pszBuffer )
	{
		bError = true;
		return pszBuffer;
	}

	// Store off the key
	strncpy( pszKey, token, uiKeySize );
	pszKey[ uiKeySize - 1 ] = '\0';

	// Fix keynames with trailing spaces
	size_t n = strlen( pszKey );
	while( n && pszKey[ n - 1 ] == ' ' )
	{
		pszKey[ n - 1 ] = 0;
		--n;
	}

	// Parse value	
	pszBuffer = COM_Parse( pszBuffer, token, sizeof( token ) );

	// Ran out of buffer?
	if( !pszBuffer )
	{
		bError = true;
		return pszBuffer;
	}

	// Hit the end instead of a value?
	if( token[ 0 ] == '}' )
	{
		bError = true;
		return pszBuffer;
	}

	strncpy( pszValue, token, uiValueSize );
	pszValue[ uiValueSize - 1 ] = '\0';

	// Return what's left in the stream
	return pszBuffer;
}
}
