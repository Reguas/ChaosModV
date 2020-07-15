#include <stdafx.h>

static DWORD64 m_anchorTick;

static void OnStart()
{
	m_anchorTick = GetTickCount64();

	SET_WEATHER_TYPE_OVERTIME_PERSIST("THUNDER", 2.f);
}

static void OnStop()
{
	CLEAR_WEATHER_TYPE_PERSIST();

	SET_WEATHER_TYPE_NOW("EXTRASUNNY");
}

static void OnTick()
{
	Ped playerPed = PLAYER_PED_ID();
	Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);

	for (Vehicle veh : GetAllVehs())
	{
		if (veh != playerVeh)
		{
			APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(veh, 0, 50.f, .0f, .0f, true, true, true, true);
		}
	}

	for (Object prop : GetAllProps())
	{
		APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(prop, 0, 50.f, .0f, .0f, true, true, true, true);
	}

	DWORD64 curTick = GetTickCount64();

	static DWORD64 lastTick = GetTickCount64();
	if (lastTick < curTick - 100)
	{
		lastTick = curTick;

		SHAKE_GAMEPLAY_CAM("SMALL_EXPLOSION_SHAKE", .1f);
	}

	// Make sure weather is always set to thunder after the transition
	if (m_anchorTick < curTick - 2000)
	{
		SET_WEATHER_TYPE_NOW("THUNDER");
	}

	// Random right / left steering
	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (GET_PED_IN_VEHICLE_SEAT(playerVeh, -1, 0) != playerPed)
		{
			return;
		}

		static DWORD64 timeUntilSteer = GetTickCount64();
		static bool enableDrunkSteering = false;
		static float steering;

		if (enableDrunkSteering)
		{
			SET_VEHICLE_STEER_BIAS(playerVeh, steering);
		}

		DWORD64 curTick = GetTickCount64();

		if (timeUntilSteer < curTick)
		{
			timeUntilSteer = GetTickCount64();

			if (enableDrunkSteering)
			{
				// Give player back control

				timeUntilSteer += g_random.GetRandomInt(50, 250);
			}
			else
			{
				// Take control from player

				steering = GET_RANDOM_FLOAT_IN_RANGE(-.7f, .7f);

				timeUntilSteer += g_random.GetRandomInt(50, 300);
			}

			enableDrunkSteering = !enableDrunkSteering;
		}
	}
}

static RegisterEffect registerEffect(EFFECT_TOTAL_CHAOS, OnStart, OnStop, OnTick);