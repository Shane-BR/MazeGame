#ifndef MG_COLOR_H
#define MG_COLOR_H

#include <stdint.h>

/* 32-bit colors in 0xAARRGGBB format, matching SDL_PIXELFORMAT_ARGB8888
 * (alpha = 0xFF, fully opaque). */
typedef uint32_t Color;

/* Grayscale */
#define MG_COLOR_BLACK        ((Color)0xFF000000)
#define MG_COLOR_DARK_GRAY    ((Color)0xFF404040)
#define MG_COLOR_GRAY         ((Color)0xFF808080)
#define MG_COLOR_LIGHT_GRAY   ((Color)0xFFC0C0C0)
#define MG_COLOR_WHITE        ((Color)0xFFFFFFFF)
/* Primary / secondary */
#define MG_COLOR_RED          ((Color)0xFFFF0000)
#define MG_COLOR_GREEN        ((Color)0xFF00FF00)
#define MG_COLOR_BLUE         ((Color)0xFF0000FF)
#define MG_COLOR_YELLOW       ((Color)0xFFFFFF00)
#define MG_COLOR_CYAN         ((Color)0xFF00FFFF)
#define MG_COLOR_MAGENTA      ((Color)0xFFFF00FF)
/* Reds / pinks */
#define MG_COLOR_DARK_RED     ((Color)0xFF8B0000)
#define MG_COLOR_MAROON       ((Color)0xFF800000)
#define MG_COLOR_CRIMSON      ((Color)0xFFDC143C)
#define MG_COLOR_SALMON       ((Color)0xFFFA8072)
#define MG_COLOR_PINK         ((Color)0xFFFFC0CB)
#define MG_COLOR_HOT_PINK     ((Color)0xFFFF69B4)
/* Oranges / browns */
#define MG_COLOR_ORANGE       ((Color)0xFFFFA500)
#define MG_COLOR_DARK_ORANGE  ((Color)0xFFFF8C00)
#define MG_COLOR_CORAL        ((Color)0xFFFF7F50)
#define MG_COLOR_BROWN        ((Color)0xFFA52A2A)
#define MG_COLOR_CHOCOLATE    ((Color)0xFFD2691E)
#define MG_COLOR_TAN          ((Color)0xFFD2B48C)
#define MG_COLOR_BEIGE        ((Color)0xFFF5F5DC)
/* Yellows / golds */
#define MG_COLOR_GOLD         ((Color)0xFFFFD700)
#define MG_COLOR_KHAKI        ((Color)0xFFF0E68C)
/* Greens */
#define MG_COLOR_DARK_GREEN   ((Color)0xFF006400)
#define MG_COLOR_FOREST_GREEN ((Color)0xFF228B22)
#define MG_COLOR_LIME         ((Color)0xFF32CD32)
#define MG_COLOR_OLIVE        ((Color)0xFF808000)
#define MG_COLOR_TEAL         ((Color)0xFF008080)
#define MG_COLOR_MINT         ((Color)0xFF98FF98)
/* Blues */
#define MG_COLOR_NAVY         ((Color)0xFF000080)
#define MG_COLOR_DARK_BLUE    ((Color)0xFF00008B)
#define MG_COLOR_ROYAL_BLUE   ((Color)0xFF4169E1)
#define MG_COLOR_SKY_BLUE     ((Color)0xFF87CEEB)
#define MG_COLOR_LIGHT_BLUE   ((Color)0xFFADD8E6)
#define MG_COLOR_AQUA         ((Color)0xFF00FFFF)
#define MG_COLOR_TURQUOISE    ((Color)0xFF40E0D0)
/* Purples */
#define MG_COLOR_PURPLE       ((Color)0xFF800080)
#define MG_COLOR_INDIGO       ((Color)0xFF4B0082)
#define MG_COLOR_VIOLET       ((Color)0xFFEE82EE)
#define MG_COLOR_LAVENDER     ((Color)0xFFE6E6FA)

static inline Color getColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((Color)a << 24) | ((Color)r << 16) | ((Color)g << 8) | (Color)b;
}

#endif