#include "video.h"

#include "genlib.h"
#include "shorttypes.h"

#include "fonts/cp437_8x8.h"
#include "fonts/cp437_8x16.h"
#include "fonts/atari_4x8.h"
#include "fonts/nes_16x16.h"
#include "fonts/font_4x6.h"

#include <stdarg.h>

SDL_Window * window;
SDL_Renderer * renderer;

static void CleanUp(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void V_InitVideo(video_info_t * info) {
    if ( !SDL_WasInit(SDL_INIT_VIDEO) ) {
        if ( SDL_InitSubSystem(SDL_INIT_VIDEO) != 0 ) {
            Error("could not init SDL video subsystem: %s", SDL_GetError());
        }
    }

    atexit(CleanUp);

    video_info_t _info;
    if ( info == NULL ) {
        _info = (video_info_t){
            .title = "",
            .window_x = SDL_WINDOWPOS_CENTERED,
            .window_y = SDL_WINDOWPOS_CENTERED,
            .window_width = 640,
            .window_height = 480,
            .window_flags = 0,
            .render_flags = 0
        };
    } else {
        _info = *info;
    }

    window = SDL_CreateWindow
    (   _info.title,
        _info.window_x == 0 ? SDL_WINDOWPOS_CENTERED : _info.window_x,
        _info.window_y == 0 ? SDL_WINDOWPOS_CENTERED : _info.window_y,
        _info.window_width == 0 ? 640 : _info.window_width,
        _info.window_height == 0 ? 480 : _info.window_height,
        _info.window_flags );

    if ( window == NULL ) {
        Error("could not create window: %s", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, _info.render_flags);

    if ( renderer == NULL ) {
        Error("could not create renderer: %s", SDL_GetError());
    }
}

video_info_t V_GetInfo(void)
{
    video_info_t info;
    info.title = SDL_GetWindowTitle(window);
    SDL_GetWindowPosition(window, &info.window_x, &info.window_y);
    SDL_GetWindowSize(window, &info.window_width, &info.window_height);
    info.window_flags = SDL_GetWindowFlags(window);

    return info;
}

void V_GoFullscreen(fullscreen_t mode)
{
    SDL_SetWindowFullscreen(window, mode);
}

void V_GoWindowed(void)
{
    SDL_SetWindowFullscreen(window, 0);
}

void V_ToggleFullscreen(fullscreen_t mode)
{
    u32 flags = SDL_GetWindowFlags(window);

    if (   flags & SDL_WINDOW_FULLSCREEN
        || flags & SDL_WINDOW_FULLSCREEN_DESKTOP )
    {
        V_GoWindowed();
    } else {
        V_GoFullscreen(mode);
    }
}

// midpoint circle algorithm
void V_DrawCircle (int x0, int y0, int radius)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    SDL_RenderDrawPoint(renderer, x0, y0 + radius);
    SDL_RenderDrawPoint(renderer, x0, y0 - radius);
    SDL_RenderDrawPoint(renderer, x0 + radius, y0);
    SDL_RenderDrawPoint(renderer, x0 - radius, y0);

    while ( x < y ) {

        if ( f >= 0 ) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
    }
}

SDL_Texture * V_CreateTexture(int w, int h)
{
    SDL_Texture * texture = SDL_CreateTexture
    (   renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h );

    if ( texture == NULL ) {
        Error("could not create texture (%s)\n", SDL_GetError());
    }

    return texture;
}

extern inline void V_Clear(void);
extern inline void V_ClearRGB(u8 r, u8 g, u8 b);
extern inline void V_Refresh(void);
extern inline void V_DrawRect(SDL_Rect * rect);
extern inline void V_FillRect(SDL_Rect * rect);
extern inline void V_DrawPoint(int x, int y);
extern inline void V_DrawVLine(int x, int y1, int y2);
extern inline void V_DrawHLine(int x1, int x2, int y);
extern inline void V_SetRGBA(u8 r, u8 g, u8 b, u8 a);
extern inline void V_SetRGB(u8 r, u8 g, u8 b);
extern inline void V_SetColor(SDL_Color color);
extern inline void V_SetGray(u8 gray);
extern inline void V_DrawTexture(SDL_Texture * t, SDL_Rect * src, SDL_Rect * dst);
extern inline void V_DrawTextureFlip
(   SDL_Texture * texture,
    SDL_Rect * src,
    SDL_Rect * dst,
    SDL_RendererFlip flip );

#pragma mark - TEXT

typedef struct {
    int width;
    int height;
    const unsigned char * data;
} font_info_t;

static const font_info_t info[] = {
    [FONT_ATARI_4X8]    = {  4,  8, atari_4x8   },
    [FONT_CP437_8X8]    = {  8,  8, cp437_8x8   },
    [FONT_CP437_8X16]   = {  8, 16, cp437_8x16  },
    [FONT_NES_16X16]    = { 16, 16, nes_16x16   },

// https://hackaday.io/project/6309-vga-graphics-over-spi-and-serial-vgatonic/log/20759-a-tiny-4x6-pixel-font-that-will-fit-on-almost-any-microcontroller-license-mit
    [FONT_4X6]          = {  4,  6, font_4x6    },
};

// settings for text rendering
static font_t   font        = FONT_CP437_8X16;
static float    scaleX      = 1.0f; // horizontal draw scale
static float    scaleY      = 1.0f; // vertical draw scale
static int      tabSize     = 4;

void V_SetFont(font_t _font)
{
    font = _font;
}

void V_SetTextScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void V_SetTabSize(int size)
{
    tabSize = size;
}

int V_CharWidth(void)
{
    return info[font].width * scaleX;
}

int V_CharHeight(void)
{
    return info[font].height * scaleY;
}

void V_PrintChar(int x, int y, unsigned char character)
{
    if ( renderer == NULL ) {
        Error("no font renderer is set, use SetFontRenderer()");
    }

    // save the current render scale
    float oldScaleX;
    float oldScaleY;
    SDL_RenderGetScale(renderer, &oldScaleX, &oldScaleY);
    SDL_RenderSetScale(renderer, scaleX * oldScaleX, scaleY * oldScaleY);

    // scale drawing but not coordinates
    const int unscaledX = (float)x / scaleX;
    const int unscaledY = (float)y / scaleY;

    const int w = info[font].width;
    const int h = info[font].height;

    int bytesPerChar = 0;
    if ( w < 8 ) {
        bytesPerChar = h;
    } else {
        bytesPerChar = (w * h) / 8;
    }

    const u8 * data = &info[font].data[character * bytesPerChar];
    int bit = 7;

    for ( int row = 0; row < h; row++ ) {
        for ( int col = 0; col < w; col++ ) {

            if ( *data & (1 << bit) ) {
                SDL_RenderDrawPoint(renderer, unscaledX + col, unscaledY + row);
            }

            if ( --bit < 8 - w ) {
                ++data;
                bit = 7;
            }
        }
    }

    // restore previous render scale
    SDL_RenderSetScale(renderer, oldScaleX, oldScaleY);
}

int V_PrintString(int x, int y, const char * format, ...)
{
    va_list args[2];
    va_start(args[0], format);
    va_copy(args[1], args[0]);

    int len = vsnprintf(NULL, 0, format, args[0]);
    char * buffer = calloc(len + 1, sizeof(*buffer));
    vsnprintf(buffer, len + 1, format, args[1]);
    va_end(args[0]);
    va_end(args[1]);

    const char * c = buffer;
    int x1 = x;
    int y1 = y;
    int w = info[font].width * scaleX;
    int h = info[font].height * scaleY;

    while ( *c ) {
        switch ( *c ) {
            case '\n':
                y1 += h;
                x1 = x;
                break;
            case '\t':
                while ( (++x1 - x) % (tabSize * w) != 0 )
                    ;
                break;
            default:
                V_PrintChar(x1, y1, *c);
                x1 += w;
                break;
        }

        c++;
    }

    free(buffer);
    return x1;
}
