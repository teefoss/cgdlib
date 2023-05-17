// -----------------------------------------------------------------------------
// Math Library
//
// General math macros, geometry, vectors, random number generator, and noise.
// -----------------------------------------------------------------------------
#ifndef __MATHLIB_H__
#define __MATHLIB_H__

#include "genlib.h"
#include "vector.h"
#include "shorttypes.h"
#include <stdbool.h>

#define RANDOM_MAX          0xFFFFFFFF
#define RANDOM_INDEX(arr)   Random(0, sizeof(arr) / sizeof(arr[0]) - 1)
#define RANDOM_ELEMENT(arr) arr[RANDOM_INDEX(arr)]

#define CLAMP(a, min, max) if (a<min) { a=min; } else if (a>max) { a=max; }
#define SIGN(a) (a < 0 ? -1 : a > 0 ? 1 : 0)
#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))
#define DEG2RAD(a) ( ((a) * M_PI) / 180.0f )
#define RAD2DEG(a) ( ((a) * 180.0f) / M_PI )
#define SQR(x) ((x) * (x))
#define DISTANCE(x1, y1, x2, y2) sqrt(SQR(x2 - x1) + SQR(y2 - y1))

// map x, which is in the range `a...b` to the range `c...d`
#define MAP(x, a, b, c, d) ((x - a) * (d - c) / (b - a) + c)

// TODO: this
#define ADD_2D(a, b) (typeof(a)){ a.x + b.x, a.y + b.y }
// ...

typedef struct { int left, top, right, bottom; } Box;

typedef enum {
    RECT_SIDE_TOP,
    RECT_SIDE_BOTTOM,
    RECT_SIDE_LEFT,
    RECT_SIDE_RIGHT,

    NUM_RECT_SIDES,
    RECT_SIDE_NONE,
} rect_side_t;

/// Linearly interpolate from a to b with interpolation point w.
float Lerp(float a, float b, float w);
float LerpEpsilon(float a, float b, float w, float epsilon);

/// Test for line-circle interaction.
/// - Parameter p1: Start of line.
/// - Parameter p2: End of line.
/// - Parameter cp: Center of circle.
/// - Parameter r: Circle radius.
/// - Returns: Whether the line intersects with the circle.
bool LineCircleIntersection
(   vec2_t p1,
    vec2_t p2,
    vec2_t cp,
    double r
/*, bool segment */ );

/// For a line draw from `point` to `rect` center, return which side of `rect`
/// is intersected.
rect_side_t RectSideLineIntersection(vec2_t point, SDL_Rect rect);

int RectDiagonalLength(int w, int h);
bool RectInRect(SDL_Rect a, SDL_Rect b);
bool RectsIntersect(SDL_Rect a, SDL_Rect b);
bool LinesIntersect
(   vec2_t a1, vec2_t a2,
    vec2_t b1, vec2_t b2,
    vec2_t * intersection);

bool PointInRect(int x, int y, SDL_Rect r);
bool RectsOverlap(SDL_Rect a, SDL_Rect b);
int ManhattanDistance(int x1, int y1, int x2, int y2);

void    WrapPosition(vec2_t * position, int w, int h);

#pragma mark - RANDOM

/// Rotate a vector by a random amount.
/// - Parameter v: The vector to rotate.
/// - Parameter degrees: The range (+/- degrees) in which
///   the vector will be rotated.
void RandomizeVector(vec2_t * v, float degrees);

vec2_t  RandomDirection(void);

/// Random velocity in a random direction.
vec2_t  RandomVelocity(float min, float max);

void SeedRandom(u32 newSeed);

/// Seed the random number generator with `time()`.
void Randomize(void);

u32 Random32(void);

/// Generate a random int between min and max, inclusive
u32 Random(u32 min, u32 max);

/// Generate a random float between min and max, inclusive
float RandomFloat(float min, float max);

/// `percent` between 0 and 1
bool Chance(float percent);

#pragma mark - NOISE

void RandomizeNoise(int seed);

/// Perlin noise at point x, y, z.
/// - Parameter frequency: Scaling value.
/// - Parameter persistense: `0.0...1.0`
/// - Parameter lacunarity: > `1.0`
float Noise2
(   float x,
    float y,
    float z,
    float frequency,
    int   octaves,
    float amplitude,
    float persistence,
    float lacunarity );

/// Perlin noise at point x, y, z. Uses common default noise parameters. Use
/// `Perlin2` if you need to specify these parameters.
inline float Noise(float x, float y, float z)
{
    return Noise2(x, y, z, 0.01, 6, 1.0, 0.5, 2.0);
}

#endif /* __MATHLIB_H__ */
