#ifndef MG_MATH_UTILS_H
#define MG_MATH_UTILS_H

#define PI 3.14159265358979323846
#define RAD(x) ((x)*(PI/180.0))

typedef struct 
{
    float x, y;
} Vec2f;

typedef struct 
{
    int x, y;
} Vec2i;

static inline void vec2f_scale (Vec2f *vec, float scalar)
{
    vec->x *= scalar; 
    vec->y *= scalar; 
}

static inline void vec2f_negate (Vec2f *vec)
{
    vec->x = -vec->x; 
    vec->y = -vec->y; 
}

static inline void vec2f_add (Vec2f a, Vec2f b, Vec2f *result)
{
    result->x = a.x + b.x; 
    result->y = a.y + b.y; 
}

#endif