/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
#if !defined ( DEMO_APIH )
#define DEMO_APIH
#ifdef _WIN32
#pragma once
#endif

struct demo_api_t
{
	/**
	*	@return Whether a demo is currently being recorded.
	*/
	int		( *IsRecording )	( void );

	/**
	*	@return Whether a demo is currently being played back.
	*/
	int		( *IsPlayingback )	( void );

	/**
	*	@return Whether this is a time demo.
	*/
	int		( *IsTimeDemo )		( void );

	/**
	*	Writes data to the demo buffer for this frame.
	*/
	void	( *WriteBuffer )	( int size, unsigned char* pBuffer );
};

extern demo_api_t demoapi;

#endif
