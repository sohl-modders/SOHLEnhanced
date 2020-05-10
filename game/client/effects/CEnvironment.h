#ifndef GAME_CLIENT_EFFECTS_CENVIRONMENT_H
#define GAME_CLIENT_EFFECTS_CENVIRONMENT_H

#include "Weather.h"

/**
*	Class that manages environmental effects.
*/
class CEnvironment final
{
public:
	CEnvironment() = default;

	WeatherType::WeatherType GetWeatherType() const { return m_WeatherType; }

	void SetWeatherType( const WeatherType::WeatherType type );

	model_t* GetRainModel() { return m_pRainSprite; }
	model_t* GetRippleModel() { return m_pRipple; }
	model_t* GetRainSplashModel() { return m_pRainSplash; }

	float GetOldTime() const { return m_flOldTime; }

	void RegisterParticleClasses();

	void Initialize();

	void Update();

private:
	void SetupGrass();

	void UpdateWind();

	void UpdateGrass();

	void UpdateRain();

	void UpdateSnow();

	void CreateGrassPiece( const Vector& vecOrigin, const Vector& vecNormal );

	void CreateSnowFlake( const Vector& vecOrigin );

	void CreateRaindrop( const Vector& vecOrigin );

	void CreateWindParticle( const Vector& vecOrigin );

private:
	WeatherType::WeatherType m_WeatherType = WeatherType::NONE;

	bool m_bGrassActive;

	Vector m_vecWeatherOrigin;

	float m_flWeatherTime;
	Vector m_vecGrassOrigin;
	float m_flGrassTime;

	model_t* m_pSnowSprite;
	model_t* m_pRainSprite;
	model_t* m_pRipple;
	model_t* m_pRainSplash;
	model_t* m_pGrassSprite1;
	model_t* m_pGrassSprite2;
	model_t* m_pGasPuffSprite;

	Vector m_vecWind;

	Vector m_vecDesiredWindDirection;

	float m_flDesiredWindSpeed;
	float m_flNextWindChangeTime;

	float m_flOldTime;

	float m_flIdealYaw;
	float m_flWeatherValue;

private:
	CEnvironment( const CEnvironment& ) = delete;
	CEnvironment& operator=( const CEnvironment& ) = delete;
};

extern CEnvironment g_Environment;

extern cvar_t* cl_weather;

#endif //GAME_CLIENT_EFFECTS_CENVIRONMENT_H