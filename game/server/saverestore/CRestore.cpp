#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CRestore.h"

bool CRestore::ReadEntVars( const char *pname, entvars_t *pev )
{
	return ReadFields( pname, pev, gEntvarsDataMap, gEntvarsDataMap.pTypeDesc, gEntvarsDataMap.uiNumDescriptors );
}

bool CRestore::ReadFields( const char *pname, void *pBaseData, const DataMap_t& dataMap, const TYPEDESCRIPTION *pFields, int fieldCount )
{
	unsigned short	i, token;
	int		lastField, fileCount;
	HEADER	header;

	i = ReadShort();
	ASSERT( i == sizeof( int ) );			// First entry should be an int

	token = ReadShort();

	// Check the struct name
	if( token != TokenHash( pname ) )			// Field Set marker
	{
		//		ALERT( at_error, "Expected %s found %s!\n", pname, BufferPointer() );
		BufferRewind( 2 * sizeof( short ) );
		return false;
	}

	// Skip over the struct name
	fileCount = ReadInt();						// Read field count

	lastField = 0;								// Make searches faster, most data is read/written in the same order

												// Clear out base data
	for( i = 0; i < fieldCount; i++ )
	{
		// Don't clear global fields
		//Only clear out fields that are actually used for save/restore. - Solokiller
		if( ( pFields[ i ].flags & TypeDescFlag::SAVE ) && ( !m_global || !( pFields[ i ].flags & TypeDescFlag::GLOBAL ) ) )
			memset( ( ( char * ) pBaseData + pFields[ i ].fieldOffset ), 0, pFields[ i ].fieldSize * g_SaveRestoreSizes[ pFields[ i ].fieldType ] );
	}

	for( i = 0; i < fileCount; i++ )
	{
		BufferReadHeader( &header );
		lastField = ReadField( pBaseData, dataMap, pFields, fieldCount, lastField, header.size, m_pdata->pTokens[ header.token ], header.pData );
		lastField++;
	}

	return true;
}

int CRestore::ReadField( void *pBaseData, const DataMap_t& dataMap, const TYPEDESCRIPTION *pFields, int fieldCount, int startField, int size, char *pName, void *pData )
{
	int i, j, stringCount, fieldNumber, entityIndex;
	const TYPEDESCRIPTION *pTest;
	float	time, timeData;
	Vector	position;
	edict_t	*pent;
	char	*pString;

	time = 0;
	position = Vector( 0, 0, 0 );

	if( m_pdata )
	{
		time = m_pdata->time;
		if( m_pdata->fUseLandmark )
			position = m_pdata->vecLandmarkOffset;
	}

	for( i = 0; i < fieldCount; i++ )
	{
		fieldNumber = ( i + startField ) % fieldCount;
		pTest = &pFields[ fieldNumber ];

		//Only check fields marked for save/restore - Solokiller
		if( ( pTest->flags & TypeDescFlag::SAVE ) && !stricmp( pTest->fieldName, pName ) )
		{
			if( !m_global || !( pTest->flags & TypeDescFlag::GLOBAL ) )
			{
				for( j = 0; j < pTest->fieldSize; j++ )
				{
					void *pOutputData = ( ( char * ) pBaseData + pTest->fieldOffset + ( j*g_SaveRestoreSizes[ pTest->fieldType ] ) );
					void *pInputData = ( char * ) pData + j * g_SaveRestoreSizes[ pTest->fieldType ];

					switch( pTest->fieldType )
					{
					case FIELD_TIME:
						timeData = *( float * ) pInputData;
						// Re-base time variables
						timeData += time;
						*( ( float * ) pOutputData ) = timeData;
						break;
					case FIELD_FLOAT:
						*( ( float * ) pOutputData ) = *( float * ) pInputData;
						break;
					case FIELD_MODELNAME:
					case FIELD_SOUNDNAME:
					case FIELD_STRING:
						// Skip over j strings
						pString = ( char * ) pData;
						for( stringCount = 0; stringCount < j; stringCount++ )
						{
							while( *pString )
								pString++;
							pString++;
						}
						pInputData = pString;
						if( strlen( ( char * ) pInputData ) == 0 )
							*( ( int * ) pOutputData ) = 0;
						else
						{
							int string;

							string = ALLOC_STRING( ( char * ) pInputData );

							*( ( int * ) pOutputData ) = string;

							if( !FStringNull( string ) && m_precache )
							{
								if( pTest->fieldType == FIELD_MODELNAME )
									PRECACHE_MODEL( ( char * ) STRING( string ) );
								else if( pTest->fieldType == FIELD_SOUNDNAME )
									PRECACHE_SOUND( ( char * ) STRING( string ) );
							}
						}
						break;
					case FIELD_EVARS:
						entityIndex = *( int * ) pInputData;
						pent = EntityFromIndex( entityIndex );
						if( pent )
							*( ( entvars_t ** ) pOutputData ) = VARS( pent );
						else
							*( ( entvars_t ** ) pOutputData ) = NULL;
						break;
					case FIELD_CLASSPTR:
						entityIndex = *( int * ) pInputData;
						pent = EntityFromIndex( entityIndex );
						if( pent )
							*( ( CBaseEntity ** ) pOutputData ) = CBaseEntity::Instance( pent );
						else
							*( ( CBaseEntity ** ) pOutputData ) = NULL;
						break;
					case FIELD_EDICT:
						entityIndex = *( int * ) pInputData;
						pent = EntityFromIndex( entityIndex );
						*( ( edict_t ** ) pOutputData ) = pent;
						break;
					case FIELD_EHANDLE:
						// Input and Output sizes are different!
						pOutputData = ( char * ) pOutputData + j*( sizeof( EHANDLE ) - g_SaveRestoreSizes[ pTest->fieldType ] );
						entityIndex = *( int * ) pInputData;
						pent = EntityFromIndex( entityIndex );
						if( pent )
							*( ( EHANDLE * ) pOutputData ) = CBaseEntity::Instance( pent );
						else
							*( ( EHANDLE * ) pOutputData ) = NULL;
						break;
					case FIELD_ENTITY:
						entityIndex = *( int * ) pInputData;
						pent = EntityFromIndex( entityIndex );
						if( pent )
							*( ( EOFFSET * ) pOutputData ) = OFFSET( pent );
						else
							*( ( EOFFSET * ) pOutputData ) = 0;
						break;
					case FIELD_VECTOR:
						( ( float * ) pOutputData )[ 0 ] = ( ( float * ) pInputData )[ 0 ];
						( ( float * ) pOutputData )[ 1 ] = ( ( float * ) pInputData )[ 1 ];
						( ( float * ) pOutputData )[ 2 ] = ( ( float * ) pInputData )[ 2 ];
						break;
					case FIELD_POSITION_VECTOR:
						( ( float * ) pOutputData )[ 0 ] = ( ( float * ) pInputData )[ 0 ] + position.x;
						( ( float * ) pOutputData )[ 1 ] = ( ( float * ) pInputData )[ 1 ] + position.y;
						( ( float * ) pOutputData )[ 2 ] = ( ( float * ) pInputData )[ 2 ] + position.z;
						break;

					case FIELD_BOOLEAN:
						*( ( bool* ) pOutputData ) = *( bool* ) pInputData;
						break;

					case FIELD_INTEGER:
						*( ( int * ) pOutputData ) = *( int * ) pInputData;
						break;

					case FIELD_SHORT:
						*( ( short * ) pOutputData ) = *( short * ) pInputData;
						break;

					case FIELD_CHARACTER:
						*( ( char * ) pOutputData ) = *( char * ) pInputData;
						break;

					case FIELD_FUNCPTR:
						if( strlen( ( char * ) pInputData ) == 0 )
							*( ( int * ) pOutputData ) = 0;
						else
						{
							//All member functions pointers should have the same size, so this should work fine. - Solokiller
							*( ( BASEPTR * ) pOutputData ) = UTIL_FunctionFromName( dataMap, ( const char* ) pInputData );
						}
						break;

					default:
						ALERT( at_error, "Bad field type\n" );
					}
				}
			}
#if 0
			else
			{
				ALERT( at_console, "Skipping global field %s\n", pName );
			}
#endif
			return fieldNumber;
		}
	}

	return -1;
}

int	CRestore::ReadInt( void )
{
	int tmp = 0;

	BufferReadBytes( ( char * ) &tmp, sizeof( int ) );

	return tmp;
}

short CRestore::ReadShort( void )
{
	short tmp = 0;

	BufferReadBytes( ( char * ) &tmp, sizeof( short ) );

	return tmp;
}

int CRestore::ReadNamedInt( const char *pName )
{
	HEADER header;

	BufferReadHeader( &header );
	return ( ( int * ) header.pData )[ 0 ];
}

char *CRestore::ReadNamedString( const char *pName )
{
	HEADER header;

	BufferReadHeader( &header );
#ifdef TOKENIZE
	return ( char * ) ( m_pdata->pTokens[ *( short * ) header.pData ] );
#else
	return ( char * ) header.pData;
#endif
}

char *CRestore::BufferPointer( void )
{
	if( !m_pdata )
		return NULL;

	return m_pdata->pCurrentData;
}

void CRestore::BufferReadBytes( char *pOutput, int size )
{
	ASSERT( m_pdata != NULL );

	if( !m_pdata || Empty() )
		return;

	if( ( m_pdata->size + size ) > m_pdata->bufferSize )
	{
		ALERT( at_error, "Restore overflow!" );
		m_pdata->size = m_pdata->bufferSize;
		return;
	}

	if( pOutput )
		memcpy( pOutput, m_pdata->pCurrentData, size );
	m_pdata->pCurrentData += size;
	m_pdata->size += size;
}

void CRestore::BufferSkipBytes( int bytes )
{
	BufferReadBytes( NULL, bytes );
}

int CRestore::BufferSkipZString( void )
{
	char *pszSearch;
	int	 len;

	if( !m_pdata )
		return 0;

	int maxLen = m_pdata->bufferSize - m_pdata->size;

	len = 0;
	pszSearch = m_pdata->pCurrentData;
	while( *pszSearch++ && len < maxLen )
		len++;

	len++;

	BufferSkipBytes( len );

	return len;
}

int	CRestore::BufferCheckZString( const char *string )
{
	if( !m_pdata )
		return 0;

	int maxLen = m_pdata->bufferSize - m_pdata->size;
	int len = strlen( string );
	if( len <= maxLen )
	{
		if( !strncmp( string, m_pdata->pCurrentData, len ) )
			return 1;
	}
	return 0;
}

void CRestore::BufferReadHeader( HEADER *pheader )
{
	ASSERT( pheader != NULL );
	pheader->size = ReadShort();				// Read field size
	pheader->token = ReadShort();				// Read field name token
	pheader->pData = BufferPointer();			// Field Data is next
	BufferSkipBytes( pheader->size );			// Advance to next field
}