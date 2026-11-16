#ifndef MG_COLOR_H
#define MG_COLOR_H

#include <stdint.h>

/* 32-bit colors in 0xRRGGBBAA format, matching SDL_PIXELFORMAT_RGBA8888
 * (alpha = 0xFF, fully opaque). */
 typedef uint32_t Color;
 
/* Grayscale */
#define MG_COLOR_BLACK        ((uint32_t)0x000000FF)
#define MG_COLOR_DARK_GRAY    ((uint32_t)0x404040FF)
#define MG_COLOR_GRAY         ((uint32_t)0x808080FF)
#define MG_COLOR_LIGHT_GRAY   ((uint32_t)0xC0C0C0FF)
#define MG_COLOR_WHITE        ((uint32_t)0xFFFFFFFF)
 
/* Primary / secondary */
#define MG_COLOR_RED          ((uint32_t)0xFF0000FF)
#define MG_COLOR_GREEN        ((uint32_t)0x00FF00FF)
#define MG_COLOR_BLUE         ((uint32_t)0x0000FFFF)
#define MG_COLOR_YELLOW       ((uint32_t)0xFFFF00FF)
#define MG_COLOR_CYAN         ((uint32_t)0x00FFFFFF)
#define MG_COLOR_MAGENTA      ((uint32_t)0xFF00FFFF)
 
/* Reds / pinks */
#define MG_COLOR_DARK_RED     ((uint32_t)0x8B0000FF)
#define MG_COLOR_MAROON       ((uint32_t)0x800000FF)
#define MG_COLOR_CRIMSON      ((uint32_t)0xDC143CFF)
#define MG_COLOR_SALMON       ((uint32_t)0xFA8072FF)
#define MG_COLOR_PINK         ((uint32_t)0xFFC0CBFF)
#define MG_COLOR_HOT_PINK     ((uint32_t)0xFF69B4FF)
 
/* Oranges / browns */
#define MG_COLOR_ORANGE       ((uint32_t)0xFFA500FF)
#define MG_COLOR_DARK_ORANGE  ((uint32_t)0xFF8C00FF)
#define MG_COLOR_CORAL        ((uint32_t)0xFF7F50FF)
#define MG_COLOR_BROWN        ((uint32_t)0xA52A2AFF)
#define MG_COLOR_CHOCOLATE    ((uint32_t)0xD2691EFF)
#define MG_COLOR_TAN          ((uint32_t)0xD2B48CFF)
#define MG_COLOR_BEIGE        ((uint32_t)0xF5F5DCFF)
 
/* Yellows / golds */
#define MG_COLOR_GOLD         ((uint32_t)0xFFD700FF)
#define MG_COLOR_KHAKI        ((uint32_t)0xF0E68CFF)
 
/* Greens */
#define MG_COLOR_DARK_GREEN   ((uint32_t)0x006400FF)
#define MG_COLOR_FOREST_GREEN ((uint32_t)0x228B22FF)
#define MG_COLOR_LIME         ((uint32_t)0x32CD32FF)
#define MG_COLOR_OLIVE        ((uint32_t)0x808000FF)
#define MG_COLOR_TEAL         ((uint32_t)0x008080FF)
#define MG_COLOR_MINT         ((uint32_t)0x98FF98FF)
 
/* Blues */
#define MG_COLOR_NAVY         ((uint32_t)0x000080FF)
#define MG_COLOR_DARK_BLUE    ((uint32_t)0x00008BFF)
#define MG_COLOR_ROYAL_BLUE   ((uint32_t)0x4169E1FF)
#define MG_COLOR_SKY_BLUE     ((uint32_t)0x87CEEBFF)
#define MG_COLOR_LIGHT_BLUE   ((uint32_t)0xADD8E6FF)
#define MG_COLOR_AQUA         ((uint32_t)0x00FFFFFF)
#define MG_COLOR_TURQUOISE    ((uint32_t)0x40E0D0FF)
 
/* Purples */
#define MG_COLOR_PURPLE       ((uint32_t)0x800080FF)
#define MG_COLOR_INDIGO       ((uint32_t)0x4B0082FF)
#define MG_COLOR_VIOLET       ((uint32_t)0xEE82EEFF)
#define MG_COLOR_LAVENDER     ((uint32_t)0xE6E6FAFF)

static inline Color getColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return r << 24 & g << 16 & b << 8 & a;
}

#endif