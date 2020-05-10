#ifndef GAME_SHARED_WEATHER_H
#define GAME_SHARED_WEATHER_H

#include <cstdint>

/**
*	@file
*
*	Weather constants and common functions.
*/

namespace WeatherType
{
/**
*	Weather types.
*/
enum WeatherType : uint8_t
{
	NONE = 0,
	RAIN = 1,
	SNOW = 2
};
}

#endif //GAME_SHARED_WEATHER_H