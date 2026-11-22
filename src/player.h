#ifndef MG_PLAYER_H
#define MG_PLAYER_H

#include "math_utils.h"
#include <stdint.h>

typedef struct 
{
    uint8_t height;
    uint8_t fov_deg;
    float movement_speed;
    Vec2f position, direction;
}
Player;

#endif