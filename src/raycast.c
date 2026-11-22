#include "raycast.h"
#include "texture.h"
#include <math.h>

#define RAD(x) ((x)*(PI/180.0))

void raycast(Renderer *renderer, const GameState *g_state, Vec2f camera_plane)
{
    Player *player = g_state->player;

    for (int col = 0; col < renderer->width; col++)
    {
        float cam_ratio_x = 2 * (col / (float)renderer->width) - 1;
        Vec2i cur_map_pos = {(int)player->position.x, (int)player->position.y};

        float next_hor_dist;
        float next_vert_dist;

        Vec2f ray_dir = 
        {
            player->direction.x + camera_plane.x * cam_ratio_x,
            player->direction.y + camera_plane.y * cam_ratio_x
        };

        float delta_hor_dist = fabs(1 / ray_dir.x);
        float delta_vert_dist = fabs(1 / ray_dir.y);
        float perp_wall_dist;

        Vec2i step;

        bool hit = false;
        bool horizontal_intersection;

        if (ray_dir.x < 0)
        {
            step.x = -1;
            next_hor_dist = (player->position.x - cur_map_pos.x) * delta_hor_dist;
        }
        else 
        {
            step.x = 1;
            next_hor_dist = (cur_map_pos.x + 1 - player->position.x) * delta_hor_dist;
        }

        if (ray_dir.y < 0)
        {
            step.y = -1;
            next_vert_dist = (player->position.y - cur_map_pos.y) * delta_vert_dist;
        }
        else 
        {
            step.y = 1;
            next_vert_dist = (cur_map_pos.y + 1 - player->position.y) * delta_vert_dist;
        }

        while (!hit)
        {
            if (next_hor_dist < next_vert_dist)
            {
                next_hor_dist += delta_hor_dist;
                cur_map_pos.x += step.x;
                horizontal_intersection = true;
            }
            else 
            {
                next_vert_dist += delta_vert_dist;
                cur_map_pos.y += step.y;
                horizontal_intersection = false;
            }

            if (cur_map_pos.x < 0 || cur_map_pos.x >= g_state->map_width || 
                cur_map_pos.y < 0 || cur_map_pos.y >= g_state->map_height)
            {
                break;
            }

            if (getMapTile(g_state, cur_map_pos) > 0) hit = true;
        }

        if (hit == false)
        {
            break;
        }

        // Get distance to camera plane by stepping back
        if (horizontal_intersection)
        {
            perp_wall_dist = next_hor_dist - delta_hor_dist;
        }
        else 
        {
            perp_wall_dist = next_vert_dist - delta_vert_dist;
        }

        int renderer_centre = renderer->height / 2;
        int line_height = (int)(renderer->height / perp_wall_dist);

        int draw_start = renderer_centre - (line_height / 2);
        if (draw_start < 0) draw_start = 0;
        int draw_end = renderer_centre + (line_height / 2);
        if (draw_end >= renderer->height) draw_end = renderer->height-1;

        // Draw it
        int texture_id = 0;
        float wall_x;

        if (!horizontal_intersection)
        {
            wall_x = player->position.x + perp_wall_dist * ray_dir.x;
        }
        else 
        {
            wall_x = player->position.y + perp_wall_dist * ray_dir.y;
        }

        // How far along tile
        wall_x -= floor(wall_x);

        int tex_w = getTexture(texture_id)->width;
        int texture_xoffset = (int)(wall_x * tex_w);
        
        if(horizontal_intersection && ray_dir.x > 0) 
        {
            texture_xoffset = tex_w - texture_xoffset - 1;
        }
        if(!horizontal_intersection && ray_dir.y < 0) 
        {
            texture_xoffset = tex_w - texture_xoffset - 1;
        }

        drawTexturedVertLine(renderer, texture_id, texture_xoffset, draw_start, draw_end, col);

    }
}