#include "raycast.h"
#include "texture.h"
#include <math.h>


/*
    TODO: Try this
    Instead of floor or ceiling casting first and then raycasting for walls:

    1. Run the wall raycaster, and write an array with information about each wall slice:
        a. Each element should store (index is column):
            1. Texture id       (16 bits)
            2. Texure x offset  (16 bits)
            3. Start Row        (16 bits)
            4. End row          (16 bits)
        b. 

    2. Run floorcasting:
        a. When writing pixel buffer,
            if withen range specified for current column wall slice.
                draw pixel sampled from given id for wall slice
            else
                draw floor/ceiling pixel

    Don't need blitting with this setup, just write directly to window surface.
*/


static void floorcast(Renderer *renderer, const GameState *g_state, Vec2f camera_plane)
{
    Player *player = g_state->player;
    Texture *floor_tex = getTexture(1);

    for (int y = 0; y < renderer->height; y++)
    {
        Vec2f leftmost_ray = 
        {
            player->direction.x - camera_plane.x,  
            player->direction.y - camera_plane.y
        };

        Vec2f rightmost_ray = 
        {
            player->direction.x + camera_plane.x,  
            player->direction.y + camera_plane.y
        };

        float vert_pos = renderer->height / 2.0;
        int low_point = y - vert_pos;

        float floor_distance = vert_pos / low_point;

        Vec2f floor_step = 
        {
            floor_distance * (rightmost_ray.x - leftmost_ray.x) / renderer->width,
            floor_distance * (rightmost_ray.y - leftmost_ray.y) / renderer->width
        };

        Vec2f floor_pos = 
        {
            player->position.x + floor_distance * leftmost_ray.x,
            player->position.y + floor_distance * leftmost_ray.y
        };

        // Write pixels
        for (int x = 0; x < renderer->width; x++)
        {
            Vec2i grid_floor_pos = {(int)floor_pos.x, (int)floor_pos.y};

            Vec2i tex_coords = 
            {
                (int)(floor_tex->width * (floor_pos.x - grid_floor_pos.x)) & (floor_tex->width - 1),
                (int)(floor_tex->height * (floor_pos.y - grid_floor_pos.y)) & (floor_tex->height - 1)
            };

            floor_pos.x += floor_step.x;
            floor_pos.y += floor_step.y;

            drawTextureSampledPixel(renderer, 1, tex_coords.x, tex_coords.y, x, y);
            drawTextureSampledPixel(renderer, 1, tex_coords.x, tex_coords.y, x, renderer->height - y - 1);
        }
    }
}

void raycast(Renderer *renderer, const GameState *g_state, Vec2f camera_plane)
{
    Player *player = g_state->player;

    floorcast(renderer, g_state, camera_plane);

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