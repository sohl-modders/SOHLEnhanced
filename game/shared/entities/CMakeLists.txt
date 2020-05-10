add_sources(
	CBaseCombatCharacter.h
	CBaseCombatCharacter.shared.cpp
	CBaseEntity.shared.h
	CBaseEntity.shared.cpp
	CBaseForward.h
	CEntityDictionary.h
	CEntityDictionary.cpp
	CEntityRegistry.h
	CEntityRegistry.cpp
	DataMapping.h
	DataMapping.cpp
	EHandle.h
	EHandle.cpp
)

add_subdirectory( player )
add_subdirectory( weapons )