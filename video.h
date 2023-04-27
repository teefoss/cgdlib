// -----------------------------------------------------------------------------
// Video Library
//
// Window, renderer, and text.
// -----------------------------------------------------------------------------
#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "genlib.h"
#include "shorttypes.h"

#include <SDL.h>

typedef struct {            // default value:
    const char * title;     // ""
    int window_x;           // SDL_WINDOWPOS_CENTERED
    int window_y;           // SDL_WINDOWPOS_CENTERED
    int window_width;       // 640
    int window_height;      // 480
    int window_flags;       // 0
    int render_flags;       // 0
} video_info_t;

typedef enum {
    // old-school fullscreen, change desktop resolution
    FULLSCREEN = SDL_WINDOW_FULLSCREEN,
    // fake fullscreen, just make the window fill the screen
    // (favor this for 2D games)
    DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
} fullscreen_t;

extern SDL_Window * window;
extern SDL_Renderer * renderer;

/// Initialize window and renderer with options specified in `info`.
/// - Parameter info: `NULL` or Zero values indicate default values
///   should be used.
void V_InitVideo(video_info_t * info);

/// Get current information about the window.
video_info_t V_GetInfo(void);

void V_GoFullscreen(fullscreen_t mode);
void V_GoWindowed(void);
void V_ToggleFullscreen(fullscreen_t mode);

void V_DrawCircle (int x0, int y0, int radius);

/// Clear the rendering target with current draw color.
inline void V_Clear(void)
{
    SDL_RenderClear(renderer);
}

inline void V_ClearRGB(u8 r, u8 g, u8 b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
}

/// Present any rendering that was done since the previous call.
inline void V_Refresh(void)
{
    SDL_RenderPresent(renderer);
}

/// Draw a rectangle outline with the current draw color.
inline void V_DrawRect(SDL_Rect * rect)
{
    SDL_RenderDrawRect(renderer, rect);
}

/// Draw a filled rectangle with the current draw color.
inline void V_FillRect(SDL_Rect * rect)
{
    SDL_RenderFillRect(renderer, rect);
}

/// Draw a point at pixel coordinates x, y.
inline void V_DrawPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

inline void V_DrawVLine(int x, int y1, int y2)
{
    SDL_RenderDrawLine(renderer, x, y1, x, y2);
}

inline void V_DrawHLine(int x1, int x2, int y)
{
    SDL_RenderDrawLine(renderer, x1, y, x2, y);
}

/// Set the draw color.
inline void V_SetRGBA(u8 r, u8 g, u8 b, u8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

inline void V_SetRGB(u8 r, u8 g, u8 b)
{
    V_SetRGBA(r, g, b, SDL_ALPHA_OPAQUE);
}

/// Set the draw color.
inline void V_SetColor(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

/// Set the draw color.
inline void V_SetGray(u8 gray)
{
    SDL_SetRenderDrawColor(renderer, gray, gray, gray, 255);
}

/// Copy a portion of the texture to current rendering target.
/// - Parameter src: The portion of the texture to be drawn, or `NULL` to
///   draw the entire texture.
/// - Parameter dst: the location with the rending target to draw to, or `NULL`
///   to draw to entire target.
inline void V_DrawTexture(SDL_Texture * texture, SDL_Rect * src, SDL_Rect * dst)
{
    SDL_RenderCopy(renderer, texture, src, dst);
}

inline void V_DrawTextureFlip
(   SDL_Texture * texture,
    SDL_Rect * src,
    SDL_Rect * dst,
    SDL_RendererFlip flip )
{
    SDL_RenderCopyEx(renderer, texture, src, dst, 0.0, NULL, flip);
}

/// Create an SDL_Texture with that can be used as a rendering target.
SDL_Texture * V_CreateTexture(int w, int h);

// -----------------------------------------------------------------------------
// Text
//  TODO: handle sprite sheet fonts?
// -----------------------------------------------------------------------------

typedef enum {
    FONT_ATARI_4X8,
    FONT_CP437_8X16,
    FONT_CP437_8X8,
    FONT_NES_16X16,
    FONT_4X6,
} font_t;

void V_SetFont(font_t font);
void V_SetTextScale(float x, float y);
void V_SetTabSize(int size);

/// Get the current font character scaled width in pixels.
int V_CharWidth(void);

/// Get the current font character scaled height in pixels.
int V_CharHeight(void);

/// Render ASCII character at pixel coordinate (x, y) using current renderer
/// color.
void V_PrintChar(int x, int y, unsigned char character);

///  Render string at pixel coordinate (x, y) using current renderer color.
///
///  The control characters \n and \t are handled as expected.
///  Returns the x coordinate of the end of the string.
int V_PrintString(int x, int y, const char * format, ...);


#endif /* __VIDEO_H__ */
