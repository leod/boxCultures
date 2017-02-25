#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>

typedef uint16_t PlayerId; // Valid client ids are > 0
typedef uint32_t ObjectId; // Valid object ids are > 0

enum {
    PLAYER_NEUTRAL = 0
};

enum BuildingType {
    BUILDING_MAIN,
	BUILDING_HOUSE,
	BUILDING_PATH,

    BUILDING_MAX
};