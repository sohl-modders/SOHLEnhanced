/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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
#ifndef GAME_SERVER_SERVERINTERFACE_H
#define GAME_SERVER_SERVERINTERFACE_H

/**
*	@file
*	Server's interface to the engine.
*	C functions for external declarations that call the appropriate C++ methods.
*/

struct sv_blending_interface_t;
struct server_studio_api_t;
struct Matrix3x4;

/*
*	Server library initialization.
*/

#ifdef _WIN32
//The one function in Half-Life 1 that uses stdcall. Be aware of this. - Solokiller
#define GIVEFNPTRS_DLLEXPORT __stdcall
#else
#define GIVEFNPTRS_DLLEXPORT DLLEXPORT
#endif

extern "C" void GIVEFNPTRS_DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals );

/*
*	DLL_FUNCTIONS
*/

/*
*	GameDLLInit declared in Server.h
*/

int DispatchSpawn( edict_t *pent );
void DispatchThink( edict_t *pent );
void DispatchUse( edict_t *pentUsed, edict_t *pentOther );
void DispatchTouch( edict_t *pentTouched, edict_t *pentOther );
void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther );
void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd );
void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData );
int  DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
void DispatchObjectCollisionBox( edict_t *pent );

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, engine::TYPEDESCRIPTION *pFields, int fieldCount );
void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, engine::TYPEDESCRIPTION *pFields, int fieldCount );

void SaveGlobalState( SAVERESTOREDATA *pSaveData );
void RestoreGlobalState( SAVERESTOREDATA *pSaveData );
void ResetGlobalState( void );

/*
*	Client functions declared in client.h
*/

/*
*	Player physics functions declared in pm_shared.h
*/

/*
*	NEW_DLL_FUNCTIONS
*/

/*
*	GameDLLShutdown declared in Server.h
*/

void OnFreeEntPrivateData( edict_t* pEdict );

int ShouldCollide( edict_t *pentTouched, edict_t *pentOther );

void CvarValue( const edict_t *pEnt, const char *value );

void CvarValue2( const edict_t *pEnt, int requestID, const char *cvarName, const char *value );

extern "C" DLLEXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion );
extern "C" DLLEXPORT int GetEntityAPI2( DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion );

/**
*	Provides new game interfaces to the engine. Not included in the HL SDK, though Ricochet uses it.
*	This is needed in order to trigger the CBaseEntity destructor, as well as provide several other functions.
*	- Solokiller
*/
extern "C" DLLEXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS* pFunctionTable, int* pInterfaceVersion );

#if 0
/**
*	Provides a blending interface to the engine to allow the engine to set up bones on the server side to match non-standard blending on the client side. - Solokiller
*/
extern "C" DLLEXPORT int Server_GetBlendingInterface( int version, sv_blending_interface_t** ppInterface, server_studio_api_t* pStudio, Matrix3x4* pRotationMatrix, Matrix3x4* pBoneTransform );
#endif

#endif //GAME_SERVER_SERVERINTERFACE_H