#ifndef MG_COLOR_H
#define MG_COLOR_H

#include <stdint.h>

/* 32-bit colors in 0xAARRGGBB format, matching SDL_PIXELFORMAT_ARGB8888
 * (alpha = 0xFF, fully opaque). */
typedef uint32_t ARGB32;

/* Grayscale */
#define MG_COLOR_BLACK        ((ARGB32)0xFF000000)
#define MG_COLOR_DARK_GRAY    ((ARGB32)0xFF404040)
#define MG_COLOR_GRAY         ((ARGB32)0xFF808080)
#define MG_COLOR_LIGHT_GRAY   ((ARGB32)0xFFC0C0C0)
#define MG_COLOR_WHITE        ((ARGB32)0xFFFFFFFF)
/* Primary / secondary */
#define MG_COLOR_RED          ((ARGB32)0xFFFF0000)
#define MG_COLOR_GREEN        ((ARGB32)0xFF00FF00)
#define MG_COLOR_BLUE         ((ARGB32)0xFF0000FF)
#define MG_COLOR_YELLOW       ((ARGB32)0xFFFFFF00)
#define MG_COLOR_CYAN         ((ARGB32)0xFF00FFFF)
#define MG_COLOR_MAGENTA      ((ARGB32)0xFFFF00FF)
/* Reds / pinks */
#define MG_COLOR_DARK_RED     ((ARGB32)0xFF8B0000)
#define MG_COLOR_MAROON       ((ARGB32)0xFF800000)
#define MG_COLOR_CRIMSON      ((ARGB32)0xFFDC143C)
#define MG_COLOR_SALMON       ((ARGB32)0xFFFA8072)
#define MG_COLOR_PINK         ((ARGB32)0xFFFFC0CB)
#define MG_COLOR_HOT_PINK     ((ARGB32)0xFFFF69B4)
/* Oranges / browns */
#define MG_COLOR_ORANGE       ((ARGB32)0xFFFFA500)
#define MG_COLOR_DARK_ORANGE  ((ARGB32)0xFFFF8C00)
#define MG_COLOR_CORAL        ((ARGB32)0xFFFF7F50)
#define MG_COLOR_BROWN        ((ARGB32)0xFFA52A2A)
#define MG_COLOR_CHOCOLATE    ((ARGB32)0xFFD2691E)
#define MG_COLOR_TAN          ((ARGB32)0xFFD2B48C)
#define MG_COLOR_BEIGE        ((ARGB32)0xFFF5F5DC)
/* Yellows / golds */
#define MG_COLOR_GOLD         ((ARGB32)0xFFFFD700)
#define MG_COLOR_KHAKI        ((ARGB32)0xFFF0E68C)
/* Greens */
#define MG_COLOR_DARK_GREEN   ((ARGB32)0xFF006400)
#define MG_COLOR_FOREST_GREEN ((ARGB32)0xFF228B22)
#define MG_COLOR_LIME         ((ARGB32)0xFF32CD32)
#define MG_COLOR_OLIVE        ((ARGB32)0xFF808000)
#define MG_COLOR_TEAL         ((ARGB32)0xFF008080)
#define MG_COLOR_MINT         ((ARGB32)0xFF98FF98)
/* Blues */
#define MG_COLOR_NAVY         ((ARGB32)0xFF000080)
#define MG_COLOR_DARK_BLUE    ((ARGB32)0xFF00008B)
#define MG_COLOR_ROYAL_BLUE   ((ARGB32)0xFF4169E1)
#define MG_COLOR_SKY_BLUE     ((ARGB32)0xFF87CEEB)
#define MG_COLOR_LIGHT_BLUE   ((ARGB32)0xFFADD8E6)
#define MG_COLOR_AQUA         ((ARGB32)0xFF00FFFF)
#define MG_COLOR_TURQUOISE    ((ARGB32)0xFF40E0D0)
/* Purples */
#define MG_COLOR_PURPLE       ((ARGB32)0xFF800080)
#define MG_COLOR_INDIGO       ((ARGB32)0xFF4B0082)
#define MG_COLOR_VIOLET       ((ARGB32)0xFFEE82EE)
#define MG_COLOR_LAVENDER     ((ARGB32)0xFFE6E6FA)

static inline ARGB32 getColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((ARGB32)a << 24) | ((ARGB32)r << 16) | ((ARGB32)g << 8) | (ARGB32)b;
}

#endif