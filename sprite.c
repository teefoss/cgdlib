#include "mathlib.h"
#include "sprite.h"
#include "texture.h"
#include "video.h"

void DrawSprite
(   sprite_t * sprite,
    int cell_x,
    int cell_y,
    int dst_x,
    int dst_y,
    int scale,
    SDL_RendererFlip flip )
{
    int w = sprite->location.w;
    int h = sprite->location.h;

    SDL_Rect src = sprite->location;
    src.x += cell_x * w;
    src.y += cell_y * h;
    SDL_Rect dst = { dst_x, dst_y, w * scale, h * scale };

    SDL_Texture * texture = GetTexture(sprite->texture_name);

    if ( sprite->transparent ) {
        SDL_SetTextureAlphaMod(texture, sprite->alpha);
    }

    V_DrawTextureFlip(texture, &src, &dst, flip);
}

void SetSpriteColorMod(sprite_t * sprite, vec3_t color_mod)
{
    SDL_Texture * texture = GetTexture(sprite->texture_name);
    SDL_SetTextureColorMod(texture, color_mod.x, color_mod.y, color_mod.z);
}
