#include "extdll.h"
#include "util.h"
#include "sound/Sound.h"

#include "CMaterialsList.h"

bool CMaterialsList::LoadFromFile( const char* const pszFileName )
{
	ASSERT( pszFileName );

	//Zero out any data that might still be there
	m_iTextures = 0;
	memset( m_szTextureName, 0, sizeof( m_szTextureName ) );
	memset( m_chTextureType, 0, sizeof( m_chTextureType ) );

	FileHandle_t hFile = g_pFileSystem->Open( pszFileName, "r" );

	if( hFile == FILESYSTEM_INVALID_HANDLE )
		return false;

	{
		char szPath[ MAX_PATH ];

		g_pFileSystem->GetLocalPath( pszFileName, szPath, sizeof( szPath ) );

		//Provide the full path just in case the developer needs the information. - Solokiller
		ALERT( at_aiconsole, "CMaterialsList::LoadFromFile: Loading materials file \"%s\"\n", szPath );
	}

	char buffer[ 512 ];
	int i, j;

	char texType;

	int iFound;

	memset( buffer, 0, sizeof( buffer ) );

	// for each line in the file...
	while( g_pFileSystem->ReadLine( buffer, sizeof( buffer ), hFile ) && ( m_iTextures < CTEXTURESMAX ) )
	{
		// skip whitespace
		i = 0;
		while( buffer[ i ] && isspace( buffer[ i ] ) )
			i++;

		if( !buffer[ i ] )
			continue;

		// skip comment lines
		if( buffer[ i ] == '/' || !isalpha( buffer[ i ] ) )
			continue;

		// get texture type
		texType = toupper( buffer[ i++ ] );

		// skip whitespace
		while( buffer[ i ] && isspace( buffer[ i ] ) )
			i++;

		if( !buffer[ i ] )
			continue;

		// get sentence name
		j = i;
		while( buffer[ j ] && !isspace( buffer[ j ] ) )
			j++;

		if( !buffer[ j ] )
			continue;

		// null-terminate name and save in sentences array
		j = min( j, CBTEXTURENAMEMAX - 1 + i );
		buffer[ j ] = '\0';

		iFound = FindTextureLinear( &( buffer[ i ] ) );

		//Duplicate entry, just update the type. - Solokiller
		if( iFound != -1 )
		{
			//Notify the developer.
			ALERT( at_console, "CMaterialsList::LoadFromFile: Duplicate material entry for texture \"%s\": old type: \'%c\', new type: \'%c\'\n", 
				   &( buffer[ i ] ), m_chTextureType[ iFound ], texType );

			m_chTextureType[ iFound ] = texType;
		}
		else
		{
			m_chTextureType[ m_iTextures ] = texType;
			strcpy( &( m_szTextureName[ m_iTextures++ ][ 0 ] ), &( buffer[ i ] ) );
		}
	}

	g_pFileSystem->Close( hFile );

	SortTextures();

	return true;
}

char CMaterialsList::FindTextureType( const char* const pszName ) const
{
	int left = 0;
	int right = m_iTextures - 1;

	int pivot;
	int val;

	while( left <= right )
	{
		pivot = ( left + right ) / 2;

		val = strnicmp( pszName, m_szTextureName[ pivot ], CBTEXTURENAMEMAX - 1 );
		if( val == 0 )
		{
			return m_chTextureType[ pivot ];
		}
		else if( val > 0 )
		{
			left = pivot + 1;
		}
		else if( val < 0 )
		{
			right = pivot - 1;
		}
	}

	return CHAR_TEX_CONCRETE;
}

int CMaterialsList::FindTextureByType( int iPrevious, const char chType ) const
{
	for( int iIndex = iPrevious == INVALID_TEX_INDEX ? 0 : iPrevious + 1;
		 iIndex < m_iTextures; ++iIndex )
	{
		if( m_chTextureType[ iIndex ] == chType )
			return iIndex;
	}

	return INVALID_TEX_INDEX;
}

void CMaterialsList::SwapTextures( int i, int j )
{
	ASSERT( i < m_iTextures );
	ASSERT( j < m_iTextures );

	char chTemp;
	char szTemp[ CBTEXTURENAMEMAX ];

	strcpy( szTemp, m_szTextureName[ i ] );
	chTemp = m_chTextureType[ i ];

	strcpy( m_szTextureName[ i ], m_szTextureName[ j ] );
	m_chTextureType[ i ] = m_chTextureType[ j ];

	strcpy( m_szTextureName[ j ], szTemp );
	m_chTextureType[ j ] = chTemp;
}

void CMaterialsList::SortTextures()
{
	// Bubble sort, yuck, but this only occurs at startup and it's only 512 elements...
	//

	for( int i = 0; i < m_iTextures; i++ )
	{
		for( int j = i + 1; j < m_iTextures; j++ )
		{
			if( stricmp( m_szTextureName[ i ], m_szTextureName[ j ] ) > 0 )
			{
				// Swap
				//
				SwapTextures( i, j );
			}
		}
	}
}

int CMaterialsList::FindTextureLinear( const char* const pszName ) const
{
	ASSERT( pszName );

	for( int i = 0; i < m_iTextures; i++ )
	{
		if( stricmp( m_szTextureName[ i ], pszName ) == 0 )
		{
			return i;
		}
	}

	return -1;
}