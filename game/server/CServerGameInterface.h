#ifndef GAME_SERVER_CSERVERGAMEINTERFACE_H
#define GAME_SERVER_CSERVERGAMEINTERFACE_H

#include <memory>

#include "CBaseGameInterface.h"

class CServerConfig;

/**
*	The server's representation of itself.
*/
class CServerGameInterface : public CBaseGameInterface
{
public:
	CServerGameInterface() = default;
	~CServerGameInterface() = default;

	/**
	*	Initializes the server.
	*	@return true on success, false on failure.
	*/
	bool Initialize();

	/**
	*	Shuts down the server.
	*	Should be called even if Initialize returned false.
	*/
	void Shutdown();

	/**
	*	Called whenever an entity is created. Used to detect when a new map has started. - Solokiller
	*/
	void EntityCreated( entvars_t* pev );

	/**
	*	@return Whether the server is active.
	*/
	bool IsActive() const { return m_bActive; }

	bool ClientConnect( edict_t* pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ CCONNECT_REJECT_REASON_SIZE ] );

	void ClientDisconnect( edict_t* pEdict );

	void ClientKill( edict_t* pEdict );

	void ClientPutInServer( edict_t* pEntity );

	void ClientCommand( edict_t* pEntity );

	void ClientUserInfoChanged( edict_t* pEntity, char* infobuffer );

	void WorldInit();

	void Activate( edict_t* pEdictList, const int edictCount, const int clientMax );

	void Deactivate();

	void StartFrame();

	void ParmsNewLevel();

	void ParmsChangeLevel();

	void ClientPrecache();

	const char* GetGameDescription() const;

	void Sys_Error( const char* error_string );

	void PlayerCustomization( edict_t* pEntity, customization_t* pCust );

	void SpectatorConnect( edict_t* pEntity );

	void SpectatorDisconnect( edict_t* pEntity );

	void SpectatorThink( edict_t* pEntity );

private:
	bool m_bMapStartedLoading = false;
	bool m_bActive = false;

	std::unique_ptr<CServerConfig> m_ServerConfig;

private:
	CServerGameInterface( const CServerGameInterface& ) = delete;
	CServerGameInterface& operator=( const CServerGameInterface& ) = delete;
};

extern CServerGameInterface g_Server;

#endif //GAME_SERVER_CSERVERGAMEINTERFACE_H