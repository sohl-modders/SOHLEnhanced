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
#ifndef GAME_SERVER_ENGINE_SAVERESTORE_SAVERESTOREDEFS_H
#define GAME_SERVER_ENGINE_SAVERESTORE_SAVERESTOREDEFS_H

namespace engine
{
enum FIELDTYPE
{
	FIELD_FLOAT = 0,		// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_ENTITY,			// An entity offset (EOFFSET)
	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EVARS,			// EVARS *
	FIELD_EDICT,			// edict_t *, or edict_t *  (same thing)
	FIELD_VECTOR,			// Any vector
	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_POINTER,			// Arbitrary data pointer... to be removed, use an array of FIELD_CHARACTER
	FIELD_INTEGER,			// Any integer or enum
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)
	FIELD_BOOLEAN,			// boolean, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_TYPECOUNT,		// MUST BE LAST
};

#define _ENGINE_FIELD(type,name,fieldtype,count,flags)				{ fieldtype, #name, static_cast<int>( OFFSETOF(type, name) ), count, flags }
#define _ENGINE_BASEENT_FIELD( name, fieldtype, count, flags )		_ENGINE_FIELD( ThisClass, name, fieldtype, count, flags )
#define ENGINE_DEFINE_FIELD( name,fieldtype)						_ENGINE_BASEENT_FIELD( name, fieldtype, 1, 0)
#define ENGINE_DEFINE_ARRAY( name,fieldtype,count)					_ENGINE_BASEENT_FIELD( name, fieldtype, count, 0)
#define ENGINE_DEFINE_ENTITY_FIELD(name,fieldtype)					_ENGINE_FIELD(entvars_t, name, fieldtype, 1, 0 )
#define ENGINE_DEFINE_ENTITY_GLOBAL_FIELD(name,fieldtype)			_ENGINE_FIELD(entvars_t, name, fieldtype, 1, FTYPEDESC_GLOBAL )
#define ENGINE_DEFINE_GLOBAL_FIELD( name,fieldtype)					_ENGINE_BASEENT_FIELD( name, fieldtype, 1, FTYPEDESC_GLOBAL )


enum TypeDescFlag
{
	/**
	*	This field is masked for global entity save/restore
	*/
	FTYPEDESC_GLOBAL = 0x0001,
};

struct TYPEDESCRIPTION
{
	FIELDTYPE		fieldType;
	const char*		fieldName;
	int				fieldOffset;
	short			fieldSize;
	short			flags;
};
}

#endif //GAME_SERVER_ENGINE_SAVERESTORE_SAVERESTOREDEFS_H
