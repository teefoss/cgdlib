#ifndef SPRITE_H
#define SPRITE_H

#include "genlib.h"
#include "shorttypes.h"
#include "vector.h"

#include <stdbool.h>

// A Sprite / Sprite Sheet and relavent data
// In a sprite sheet, animations or tile varients should be
// laid out horizontally
typedef struct {
    const char * texture_name;
    u8 draw_order; // lower values are drawn first (in back)
    SDL_Rect location; // source rect in sprite sheet
    u8 num_frames; // If not animated, refers to the number of varients.
    u8 fps;

    /// Which axes this sprite can be flipped along (SDL_).
    SDL_RendererFlip flip;
    bool transparent;
    u8 alpha;
} sprite_t;

// TODO: dst_ -> window_coord_t
/// Draw sprite in spritesheet cell and at pixel coordinate.
///
/// - Parameter cell_x: Horizontal spritesheet cell offset.
/// - Parameter cell_y: Vertical spritesheet cell offset.
/// - Parameter dst_x: The horizontal window coordinate at which to draw.
/// - Parameter dst_y: The vertical window coordinate at which to draw.
/// - Parameter scale: The draw scale to use.
/// - Parameter flip: which flip axis to use.
void DrawSprite
(   sprite_t * sprite,
    int cell_x,
    int cell_y,
    int dst_x,
    int dst_y,
    int scale,
    SDL_RendererFlip flip );

void SetSpriteColorMod(sprite_t * sprite, vec3_t color_mod);

#endif /* SPRITE_H */
