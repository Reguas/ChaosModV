//
// effect by veloscocity
//

#include "Memory/Gravity.h"
#include <math.h>
#include <stdafx.h>

static float x, y;

static DWORD lastTick = 0;

static void OnStart()
{
	Ped playerPed = PLAYER_PED_ID();
	float rot =
	    GET_ENTITY_HEADING(IS_PED_IN_ANY_VEHICLE(playerPed, false) ? GET_VEHICLE_PED_IS_IN(playerPed, false)
	                                                               : playerPed); // Shake perpendicular to the player
	x             = SIN(360 - rot) * 1.33;
	y             = -COS(360 - rot) * 1.33;
}

static void OnTick()
{
	DWORD curTick = GetTickCount();
	if (lastTick < curTick - 500)
	{
		lastTick = curTick;
		x        = -x;
		y        = -y;

		LOG(x << " " << y);
	}

	for (auto object : GetAllProps())
	{
		Memory::ApplyForceToEntityCenterOfMass(object, 1, y, x, 0, false, false, true, false);
	}

	for (auto veh : GetAllVehs())
	{
		Memory::ApplyForceToEntityCenterOfMass(veh, 1, y, x, 0, false, false, true, false);
	}
}

// clang-format off
REGISTER_EFFECT(OnStart, nullptr, OnTick, EffectInfo
	{
		.Name = "Cocktail Shaker",
		.Id = "cocktail_shaker",
		.IsTimed = true,
        .IsShortDuration = true,
		.EffectCategory = EEffectCategory::Gravity,
	}
);
