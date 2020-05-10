#ifndef CAUTOSTRING_H
#define CAUTOSTRING_H

#include <cstring>
#include <functional>

#include "StringUtils.h"

/*
* String class that automatically manages memory
* Intended for use in containers
*/
class CAutoString
{
public:
	CAutoString()
		: m_pszString( nullptr )
#ifdef DEBUG_STRING_ALLOCATION
		, m_pszString2( nullptr )
#endif
	{
	}

	explicit CAutoString( const char* pszString )
		: m_pszString( nullptr )
#ifdef DEBUG_STRING_ALLOCATION
		, m_pszString2( nullptr )
#endif
	{
		Set( pszString );
	}

	CAutoString( const CAutoString& other )
		: m_pszString( nullptr )
#ifdef DEBUG_STRING_ALLOCATION
		, m_pszString2( nullptr )
#endif
	{
		Set( other.Get() );
	}

	CAutoString( CAutoString&& other )
		: m_pszString( other.m_pszString )
#ifdef DEBUG_STRING_ALLOCATION
		, m_pszString2( other.m_pszString2 )
#endif
	{
		other.m_pszString = nullptr;

#ifdef DEBUG_STRING_ALLOCATION
		other.m_pszString2 = nullptr;
#endif
	}

	~CAutoString()
	{
		delete[] m_pszString;

#ifdef DEBUG_STRING_ALLOCATION
		delete[] m_pszString2;
#endif
	}

	bool IsEmpty() const { return m_pszString == nullptr; }

	const char* Get() const
	{
		return m_pszString ? m_pszString : "";
	}

	operator const char*() const { return m_pszString ? m_pszString : ""; }

	void Set( const char* pszString );

	int Compare( const char* pszString ) const;

	bool operator<( const char* pszString ) const { return Compare( pszString ) < 0; }

	bool operator<( const CAutoString& other ) const { return Compare( other.m_pszString ) < 0; }

	bool operator==( const char* pszString ) const { return Compare( pszString ) == 0; }

	bool operator==( const CAutoString& other ) const { return Compare( other.m_pszString ) == 0; }

	bool operator!=( const char* pszString ) const { return Compare( pszString ) != 0; }

	bool operator!=( const CAutoString& other ) const { return Compare( other.m_pszString ) != 0; }

	CAutoString& operator=( const char* pszString ) { Set( pszString ); return *this; }

	CAutoString& operator=( const CAutoString& other )
	{
		if( this != &other )
		{
			Set( other.m_pszString );
		}

		return *this;
	}

	CAutoString& operator=( CAutoString&& other )
	{
		if( this != &other )
		{
			Set( nullptr );

			m_pszString = other.m_pszString;
			other.m_pszString = nullptr;

#ifdef DEBUG_STRING_ALLOCATION
			m_pszString2 = other.m_pszString2;
			other.m_pszString2 = nullptr;
#endif
		}

		return *this;
	}

#ifdef DEBUG_STRING_ALLOCATION
	const char* GetVerifyString() const { return m_pszString2; }

	bool VerifyIntegrity() const
	{
		return strcmp( m_pszString, m_pszString2 ) == 0;
	}
#endif

private:
	char* m_pszString;

#ifdef DEBUG_STRING_ALLOCATION
	char* m_pszString2;
#endif
};

inline void CAutoString::Set( const char* pszString )
{
	if( pszString != m_pszString )
	{
		delete[] m_pszString;

		if( pszString )
		{
			const size_t uiLength = strlen( pszString );
			m_pszString = new char[ uiLength + 1 ];

			strcpy( m_pszString, pszString );
		}
		else
			m_pszString = nullptr;

#ifdef DEBUG_STRING_ALLOCATION
		delete[] m_pszString2;

		if( !pszString )
		{
			m_pszString2 = nullptr;
			return;
		}

		const size_t uiLength = strlen( pszString );

		m_pszString2 = new char[ uiLength + 1 ];

		strncpy( m_pszString2, m_pszString, uiLength + 1 );
#endif
	}
}

inline int CAutoString::Compare( const char* pszString ) const
{
	if( !pszString )
		return m_pszString ? 1 : 0;

	if( !m_pszString )
		return -1;

	return strcmp( m_pszString, pszString );
}

namespace std
{
template<>
struct hash<CAutoString> : public std::unary_function<CAutoString, size_t>
{
	std::size_t operator()( const CAutoString& szStr ) const
	{
		return StringHash( szStr.Get() );
	}
};
}

#endif //CAUTOSTRING_H