#ifndef MG_RAYCAST_H
#define MG_RAYCAST_H

#include "game.h"
#include "render.h"

void raycast(Renderer *renderer, const GameState *g_state, Vec2f camera_plane);

#endif