//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined( NET_APIH )
#define NET_APIH
#ifdef _WIN32
#pragma once
#endif

#if !defined ( NETADRH )
#include "netadr.h"
#endif

struct net_response_t;

enum NetRequest
{
	/**
	*	Doesn't need a remote address.
	*/
	NETAPI_REQUEST_SERVERLIST	= 0,
	NETAPI_REQUEST_PING			= 1,
	NETAPI_REQUEST_RULES		= 2,
	NETAPI_REQUEST_PLAYERS		= 3,
	NETAPI_REQUEST_DETAILS		= 4,
};

enum NetApiFlag
{
	/*
	*	Set this flag for things like broadcast requests, etc. where the engine should not
	*	kill the request hook after receiving the first response
	*/
	FNETAPI_MULTIPLE_RESPONSE	= 1 << 0,
};

typedef void ( *net_api_response_func_t ) ( net_response_t *response );

enum NetResult
{
	NET_SUCCESS						= 0,
	NET_ERROR_TIMEOUT				= 1 << 0,
	NET_ERROR_PROTO_UNSUPPORTED		= 1 << 1,
	NET_ERROR_UNDEFINED				= 1 << 2,
};

struct net_adrlist_t
{
	net_adrlist_t	*next;
	netadr_t		remote_address;
};

struct net_response_t
{
	// NET_SUCCESS or an error code
	int			error;

	// Context ID
	int			context;
	// Type
	int			type;

	// Server that is responding to the request
	netadr_t	remote_address;

	// Response RTT ping time
	double		ping;
	// Key/Value pair string ( separated by backlash \ characters )
	// WARNING:  You must copy this buffer in the callback function, because it is freed
	//  by the engine right after the call!!!!
	// ALSO:  For NETAPI_REQUEST_SERVERLIST requests, this will be a pointer to a linked list of net_adrlist_t's
	void		*response;
};

struct net_status_t
{
		// Connected to remote server?  1 == yes, 0 otherwise
	int			connected; 
	// Client's IP address
	netadr_t	local_address;
	// Address of remote server
	netadr_t	remote_address;
	// Packet Loss ( as a percentage )
	int			packet_loss;
	// Latency, in seconds ( multiply by 1000.0 to get milliseconds )
	double		latency;
	// Connection time, in seconds
	double		connection_time;
	// Rate setting ( for incoming data )
	double		rate;
};

struct net_api_t
{
	// APIs
	/**
	*	Initialize networking.
	*/
	void		( *InitNetworking )( void );

	/**
	*	Query the network's status.
	*	@param[ out ] status Status.
	*/
	void		( *Status ) ( net_status_t *status );

	/**
	*	Sends a request.
	*	@param context User defined context ID.
	*	@param request Request type. @see NetRequest
	*	@param flags Flags. @see NetApiFlag
	*	@param timeout When to time out the request.
	*	@param remote_address Address to send the request to.
	*	@param response Callback to invoke when the response has been received.
	*/
	void		( *SendRequest ) ( int context, int request, int flags, double timeout, netadr_t *remote_address, net_api_response_func_t response );

	/**
	*	Cancels the request with the given context ID.
	*	@param context Context ID.
	*/
	void		( *CancelRequest ) ( int context );

	/**
	*	Cancels all requests.
	*/
	void		( *CancelAllRequests ) ( void );

	/**
	*	Converts an address to a string.
	*	@param a Address.
	*	@return Pointer to a static buffer containing the string representation of the address. Can be an empty string if the address is invalid.
	*/
	char*		( *AdrToString ) ( netadr_t* a );

	/**
	*	Compares 2 addresses.
	*	@param a First address.
	*	@param b Second address.
	*	@return true if the addresses match, false otherwise.
	*/
	int			( *CompareAdr ) ( netadr_t *a, netadr_t *b );

	/**
	*	Converts a string to an address.
	*	@param pszString String to convert.
	*	@param[ out ] a Address.
	*	@return true on success, false otherwise.
	*/
	int			( *StringToAdr ) ( const char* const pszString, netadr_t *a );

	/**
	*	Finds the value associated with the given key in the given info key buffer.
	*	@param pszBuffer Info key buffer.
	*	@param pszKey Key to search for.
	*	@return Pointer to a static buffer containing the value, or an empty string if it hasn't been found.
	*/
	const char*	( *ValueForKey ) ( const char* const pszBuffer, const char* const pszKey );

	/**
	*	Removes the given key from the given buffer.
	*	@param pszBuffer Info key buffer.
	*	@param pszKey Key to remove.
	*/
	void		( *RemoveKey ) ( const char* const pszBuffer, const char* const pszKey );

	/**
	*	Sets the value for the given key in the given buffer.
	*	@param pszBuffer Info key buffer.
	*	@param pszKey Key whose value to set.
	*	@param pszValue Value to set.
	*	@param iMaxSize Maximum size for the info key buffer.
	*/
	void		( *SetValueForKey ) ( const char* const pszBuffer, const char* const pszKey, const char* const pszValue, const int iMaxSize );
};

extern net_api_t netapi;

#endif // NET_APIH