#include <stdafx.h>

#include "Effects/Register/RegisterEffect.h"

static void OnStop()
{
	for (Vehicle veh : GetAllVehs())
		SET_VEHICLE_DOORS_LOCKED(veh, 1);
}

static void OnTick()
{
	Ped playerPed = PLAYER_PED_ID();

	if (IS_PED_IN_ANY_VEHICLE(playerPed, false))
	{
		Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, false);

		SET_VEHICLE_DOORS_LOCKED(veh, 4);
	}
}

// clang-format off
REGISTER_EFFECT(nullptr, OnStop, OnTick, 
	{
		.Name = "Lock Player Inside Vehicle",
		.Id = "playerveh_lock",
		.IsTimed = true
	}
);