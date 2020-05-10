/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  parsemsg.cpp
//
//--------------------------------------------------------------------------------------------------------------
#include "Game.h"

#include "parsemsg.h"

//--------------------------------------------------------------------------------------------------------------
CBaseBuffer::CBaseBuffer()
{
	Init( nullptr, 0 );
}

//--------------------------------------------------------------------------------------------------------------
CBaseBuffer::CBaseBuffer( unsigned char* pBuffer, const size_t uiBufferSize )
{
	Init( pBuffer, uiBufferSize );
}

//--------------------------------------------------------------------------------------------------------------
void CBaseBuffer::Init( unsigned char* pBuffer, const size_t uiBufferSize )
{
	m_pBuffer = pBuffer;
	m_uiBufferSize = uiBufferSize;
	m_uiPosition = 0;
	m_bOverflow = false;
}

bool CBaseBuffer::CheckOverflow( const size_t uiSize ) const
{
	return m_uiPosition + uiSize > m_uiBufferSize;
}

bool CBaseBuffer::Overflow( const size_t uiSize )
{
	if( CheckOverflow( uiSize ) )
	{
		m_bOverflow = true;

		return true;
	}

	return false;
}

int CBufferReader::ReadChar()
{
	static_assert( sizeof( signed char ) == 1, "BufferReader::ReadChar: sizeof( signed char ) must be 1!" );

	if( Overflow( 1 ) )
	{
		return -1;
	}

	int c = ( signed char ) m_pBuffer[ m_uiPosition++ ];

	return c;
}

int CBufferReader::ReadByte()
{
	static_assert( sizeof( unsigned char ) == 1, "BufferReader::ReadByte: sizeof( unsigned char ) must be 1!" );

	if( Overflow( 1 ) )
	{
		return -1;
	}

	int c = ( unsigned char ) m_pBuffer[ m_uiPosition++ ];

	return c;
}

int CBufferReader::ReadShort()
{
	static_assert( sizeof( short ) == 2, "BufferReader::ReadShort: sizeof( short ) must be 2!" );

	if( Overflow( 2 ) )
	{
		return -1;
	}

	int c = ( short ) ( m_pBuffer[ m_uiPosition ] + ( m_pBuffer[ m_uiPosition + 1 ] << 8 ) );

	m_uiPosition += 2;

	return c;
}

int CBufferReader::ReadWord()
{
	return ReadShort();
}

int CBufferReader::ReadLong()
{
	static_assert( sizeof( int ) == 4, "BufferReader::ReadLong: sizeof( int ) must be 4!" );

	if( Overflow( 4 ) )
	{
		return -1;
	}

	int c = m_pBuffer[ m_uiPosition ] + ( m_pBuffer[ m_uiPosition + 1 ] << 8 ) + ( m_pBuffer[ m_uiPosition + 2 ] << 16 ) + ( m_pBuffer[ m_uiPosition + 3 ] << 24 );

	m_uiPosition += 4;

	return c;
}

float CBufferReader::ReadFloat()
{
	static_assert( sizeof( float ) == 4, "BufferReader::ReadFloat: sizeof( float ) must be 4!" );

	if( Overflow( 4 ) )
	{
		return -1;
	}

	union
	{
		byte    b[ 4 ];
		float   f;
		int     l;
	} dat;

	dat.b[ 0 ] = m_pBuffer[ m_uiPosition ];
	dat.b[ 1 ] = m_pBuffer[ m_uiPosition + 1 ];
	dat.b[ 2 ] = m_pBuffer[ m_uiPosition + 2 ];
	dat.b[ 3 ] = m_pBuffer[ m_uiPosition + 3 ];
	m_uiPosition += 4;

	return dat.f;
}

char* CBufferReader::ReadString()
{
	static char string[ 2048 ];
	int c;

	string[ 0 ] = '\0';

	size_t l = 0;
	do
	{
		if( m_uiPosition + 1 > m_uiBufferSize )
			break; // no more characters

		c = ReadChar();
		if( c == -1 || c == '\0' )
			break;
		string[ l ] = c;
		l++;
	}
	while( l < sizeof( string ) - 1 );

	string[ l ] = '\0';

	return string;
}

float CBufferReader::ReadCoord()
{
	return ( float ) ( ReadShort() * ( 1.0 / 8 ) );
}

Vector CBufferReader::ReadCoordVector()
{
	Vector vec;

	vec.x = ReadCoord();
	vec.y = ReadCoord();
	vec.z = ReadCoord();

	return vec;
}

float CBufferReader::ReadAngle()
{
	return ( float ) ( ReadChar() * ( 360.0 / 256 ) );
}

float CBufferReader::ReadHiResAngle()
{
	return ( float ) ( ReadShort() * ( 360.0 / 65536 ) );
}

//--------------------------------------------------------------------------------------------------------------
void BufferWriter::WriteByte( unsigned char data )
{
	if (!m_pBuffer || !GetSpaceLeft())
	{
		m_bOverflow = true;
		return;
	}

	m_pBuffer[ m_uiPosition++ ] = data;
}

//--------------------------------------------------------------------------------------------------------------
void BufferWriter::WriteLong( int data )
{
	if (!m_pBuffer || GetSpaceLeft() < 4)
	{
		m_bOverflow = true;
		return;
	}

	m_pBuffer[ m_uiPosition ] = data&0xff;
	m_pBuffer[ m_uiPosition + 1 ] = (data>>8)&0xff;
	m_pBuffer[ m_uiPosition + 2 ] = (data>>16)&0xff;
	m_pBuffer[ m_uiPosition + 3 ] = data>>24;
	m_uiPosition += 4;
}

//--------------------------------------------------------------------------------------------------------------
void BufferWriter::WriteString( const char* pszString )
{
	if( !m_pBuffer || !GetSpaceLeft() )
	{
		m_bOverflow = true;
		return;
	}

	if( !pszString )
		pszString = "";

	size_t len = strlen( pszString ) + 1;
	if ( len > GetSpaceLeft() )
	{
		m_bOverflow = true;
		pszString = "";
		len = 1;
	}

	strcpy( (char *) m_pBuffer, pszString );
	m_uiPosition += len;
}