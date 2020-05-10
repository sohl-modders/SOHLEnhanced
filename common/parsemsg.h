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
//  parsemsg.h
//	MDC - copying from cstrike\cl_dll so career-mode stuff can catch messages
//  in this dll. (and C++ifying it)
//

#ifndef PARSEMSG_H
#define PARSEMSG_H

/**
*	Base class for byte buffers.
*/
class CBaseBuffer
{
public:
	/**
	*	Initializes the buffer with no backing buffer to use.
	*/
	CBaseBuffer();

	/**
	*	Initializes the buffer with the given backing buffer.
	*	@param pBuffer Buffer to use.
	*	@param uiBufferSize Size of the buffer, in bytes.
	*/
	CBaseBuffer( unsigned char* pBuffer, const size_t uiBufferSize );

	/**
	*	Constructor for engine buffer data.
	*/
	CBaseBuffer( void* pBuffer, const int iBufferSize )
		: CBaseBuffer( reinterpret_cast<unsigned char*>( pBuffer ), static_cast<size_t>( iBufferSize ) )
	{
	}

	/**
	*	@copydoc CBaseBuffer( unsigned char* pBuffer, const size_t uiBufferSize )
	*/
	void Init( unsigned char* pBuffer, const size_t uiBufferSize );

	/**
	*	@return Whether this buffer has overflowed.
	*/
	bool HasOverflowed() const { return m_bOverflow; }

	/**
	*	@return The amount of space in the buffer that has been read or written.
	*/
	size_t GetSpaceUsed() const { return m_uiPosition; }

	/**
	*	@return The amount of space that is left to be read or written.
	*/
	size_t GetSpaceLeft() const { return m_uiBufferSize - m_uiPosition; }

	/**
	*	Checks whether reading or writing uiSize bytes would cause an overflow.
	*	@param uiSize Number of bytes.
	*	@return true if an overflow would occur, false otherwise.
	*/
	bool CheckOverflow( const size_t uiSize ) const;

	/**
	*	Checks whether reading or writing uiSize bytes would cause an overflow. If so, sets the overflow flag.
	*	@param uiSize Number of bytes.
	*	@return true if an overflow has occurred, false otherwise.
	*/
	bool Overflow( const size_t uiSize );

protected:
	unsigned char* m_pBuffer;
	size_t m_uiBufferSize;
	size_t m_uiPosition;
	bool m_bOverflow;

private:
	CBaseBuffer( const CBaseBuffer& ) = delete;
	CBaseBuffer& operator=( const CBaseBuffer& ) = delete;
};

/**
*	Read buffer.
*/
class CBufferReader : public CBaseBuffer
{
public:
	using CBaseBuffer::CBaseBuffer;

	/**
	*	Reads a signed char.
	*/
	int ReadChar();

	/**
	*	Reads an unsigned char.
	*/
	int ReadByte();

	/**
	*	Reads a signed short.
	*/
	int ReadShort();

	/**
	*	Reads an unsigned short.
	*/
	int ReadWord();

	/**
	*	Reads a signed long.
	*/
	int ReadLong();

	/**
	*	Reads a float.
	*/
	float ReadFloat();

	/**
	*	Reads a string.
	*/
	char* ReadString();

	/**
	*	Reads a coordinate.
	*/
	float ReadCoord();

	/**
	*	Reads 3 coordinates.
	*/
	Vector ReadCoordVector();

	/**
	*	Reads an angle.
	*/
	float ReadAngle();

	/**
	*	Reads a high resolution angle.
	*/
	float ReadHiResAngle();
};

/**
*	Write buffer.
*/
class BufferWriter : public CBaseBuffer
{
public:
	using CBaseBuffer::CBaseBuffer;

	/**
	*	Writes an unsigned char.
	*/
	void WriteByte( unsigned char data );

	/**
	*	Writes a signed long.
	*/
	void WriteLong( int data );

	/**
	*	Writes a string.
	*/
	void WriteString( const char* pszString );
};

#endif // PARSEMSG_H



