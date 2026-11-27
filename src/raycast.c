#include "raycast.h"
#include "texture.h"

#include <SDL3/SDL_log.h>
#include <math.h>
#include <stdlib.h>


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

    Any reason we can't put the raycasting logic into the floorcasting loop
    Don't need blitting with this setup, just write directly to window surface.
*/

// TODO Handle textures better, ya bum

typedef struct 
{
    uint16_t tex_id;
    int16_t tex_x_offset;
    int16_t start_row;
    int16_t end_row;
    float tex_y_step;
} WallSlice;

static const WallSlice EMPTY_SLICE = { .start_row = 0, .end_row = 0};
static WallSlice *slices = NULL;
static float *floor_distances = NULL;

static inline bool is_empty_wall_slice(const WallSlice *slice)
{
    return slice->end_row == slice->start_row;
}

static inline bool in_slice_row_range(const WallSlice *slice, int row)
{
    return row >= slice->start_row && row <= slice->end_row;
}

static void store_wall_hit(const Window *window, const GameState *g_state, 
    const Vec2f cam_plane, int texture_id, int column);

static void init_tables(Window *window, const GameState *g_state, const Vec2f cam_plane);

void run_raycaster(Window *window, const GameState *g_state, const Vec2f cam_plane)
{

    Player *player = g_state->player;
    Texture *wall_tex = getTexture(0);
    Texture *floor_tex = getTexture(1);

    init_tables(window, g_state, cam_plane);

    Vec2f leftmost_ray = 
    {
        player->direction.x - cam_plane.x,  
        player->direction.y - cam_plane.y
    };

    Vec2f rightmost_ray = 
    {
        player->direction.x + cam_plane.x,  
        player->direction.y + cam_plane.y
    };

    for (int y = 0; y < window->height; y++)
    {
        // Floorcast
        float floor_distance = floor_distances[y];

        Vec2f floor_step = 
        {
            floor_distance * (rightmost_ray.x - leftmost_ray.x) / window->width,
            floor_distance * (rightmost_ray.y - leftmost_ray.y) / window->width
        };

        Vec2f floor_pos = 
        {
            player->position.x + floor_distance * leftmost_ray.x,
            player->position.y + floor_distance * leftmost_ray.y
        };
            
        for (int x = 0; x < window->width; x++)
        {
            // Draw pixels
            // If column has wall slice AND within range of wall slice
            Color color = 0x0;
            if (!is_empty_wall_slice(&slices[x]) && in_slice_row_range(&slices[x], y))
            {
                int tex_y = (y - slices[x].start_row) * slices[x].tex_y_step;
                color = getTexturePixel(wall_tex, slices[x].tex_x_offset, tex_y);
            }
            else 
            {
                Vec2i grid_floor_pos = {(int)floor_pos.x, (int)floor_pos.y};


                int tex_x = (int)(floor_tex->width * 
                    (floor_pos.x - grid_floor_pos.x)) & (floor_tex->width - 1);
                int tex_y = (int)(floor_tex->height * 
                    (floor_pos.y - grid_floor_pos.y)) & (floor_tex->height - 1);

                color = getTexturePixel(floor_tex, tex_x, tex_y);
                putPixel(window, color, x, window->height - y - 1);
            }

            putPixel(window, color, x, y);
            floor_pos.x += floor_step.x;
            floor_pos.y += floor_step.y;
        }
    }
}

static void store_wall_hit(const Window *window, const GameState *g_state, 
    const Vec2f cam_plane, int texture_id, int column)
{
    float cam_ratio_x = 2 * (column / (float)window->width) - 1;
    Vec2i cur_map_pos = {(int)g_state->player->position.x, (int)g_state->player->position.y};

    float next_hor_dist;
    float next_vert_dist;

    Vec2f ray_dir = 
    {
        g_state->player->direction.x + cam_plane.x * cam_ratio_x,
        g_state->player->direction.y + cam_plane.y * cam_ratio_x
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
        next_hor_dist = (g_state->player->position.x - cur_map_pos.x) * delta_hor_dist;
    }
    else 
    {
        step.x = 1;
        next_hor_dist = (cur_map_pos.x + 1 - g_state->player->position.x) * delta_hor_dist;
    }

    if (ray_dir.y < 0)
    {
        step.y = -1;
        next_vert_dist = (g_state->player->position.y - cur_map_pos.y) * delta_vert_dist;
    }
    else 
    {
        step.y = 1;
        next_vert_dist = (cur_map_pos.y + 1 - g_state->player->position.y) * delta_vert_dist;
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
        slices[column] = EMPTY_SLICE;
        return;
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

    int window_centre = window->height / 2;
    int line_height = (int)(window->height / perp_wall_dist);

    int16_t draw_start = window_centre - (line_height / 2);
    int16_t draw_end = window_centre + (line_height / 2);

    // Draw it
    float wall_x;

    if (!horizontal_intersection)
    {
        wall_x = g_state->player->position.x + perp_wall_dist * ray_dir.x;
    }
    else 
    {
        wall_x = g_state->player->position.y + perp_wall_dist * ray_dir.y;
    }

    // How far along tile
    wall_x -= floor(wall_x);

    Texture *tex = getTexture(texture_id);

    int16_t texture_xoffset = (int16_t)(wall_x * tex->width);
    
    if (horizontal_intersection && ray_dir.x > 0) 
    {
        texture_xoffset = tex->width - texture_xoffset - 1;
    }
    if (!horizontal_intersection && ray_dir.y < 0) 
    {
        texture_xoffset = tex->width - texture_xoffset - 1;
    }

    // Make entry
    float step_y = ((float)tex->height-1)/(line_height);
    WallSlice slice =
    {
        .tex_id = texture_id,
        .tex_x_offset = texture_xoffset,
        .tex_y_step = step_y,
        .start_row = draw_start,
        .end_row = draw_end
    };

    slices[column] = slice;

}

static void init_tables(Window *window, const GameState *g_state, const Vec2f cam_plane)
{
    if (slices == NULL)
    {
        slices = malloc(sizeof(*slices)* (size_t)window->width); // TODO Arena?

        if (slices == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, \
                "run_raycaster: Failed to allocate memory for wall slices tables");
            return;
        }
    }

    for (int x = 0; x < window->width; x++) 
    {
        store_wall_hit(window, g_state, cam_plane, 0, x);
    }  

    if (floor_distances == NULL)
    {
        floor_distances = malloc(sizeof(*slices)*window->height); // TODO Arena?

        if (slices == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, \
                "run_raycaster: Failed to allocate memory for floor distances table");
            return;
        }

        // TODO change when resizing
        for (int y = 0; y < window->height; y++)
        {
            float vert_pos = window->height / 2.0f;
            int low_point = y - vert_pos;
            float floor_distance = vert_pos / low_point;

            floor_distances[y] = floor_distance;
        }
    }  
}