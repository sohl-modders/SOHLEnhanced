#ifndef CHASHSTRINGPOOL_H
#define CHASHSTRINGPOOL_H

#include <unordered_set>

#include "CAutoString.h"

class CHashStringPool final
{
private:
	typedef std::unordered_set<CAutoString> Strings_t;

public:
	CHashStringPool();
	~CHashStringPool();

	const char* Find( const char* pszString ) const;

	const char* Allocate( const char* pszString, bool* pNewAllocation = nullptr );

	void Clear();

	void DebugPrint() const;

#ifdef DEBUG_STRING_ALLOCATION
	void VerifyIntegrity() const;
#endif

private:
	Strings_t m_Strings;

private:
	CHashStringPool( const CHashStringPool& ) = delete;
	CHashStringPool& operator=( const CHashStringPool& ) = delete;
};

#endif //CHASHSTRINGPOOL_H