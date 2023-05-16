#include "mathlib.h"
#include <math.h>

// https://thatfrenchgamedev.com/1026/game-programmers-handy-maths-formulas/

float Lerp(float a, float b, float w)
{
    return (1.0f - w) * a + w * b;
}

float LerpEpsilon(float a, float b, float w, float epsilon)
{
    float result = (1.0f - w) * a + w * b;
    if ( fabsf(result) < epsilon ) {
        result = b;
    }

    return result;
}

#pragma mark - GEOMETRY

//
// LineCircleIntersection
//
// Prints the intersection points (if any) of a circle, center 'cp' with
// radius 'r', and either an infinite line containing the points 'p1' and 'p2'
// or a segment drawn between those points.
//

static double sq(double x)
{
    return x * x;
}

static const double eps = 1e-14;

bool LineCircleIntersection
(   vec2_t p1,
    vec2_t p2,
    vec2_t cp,
    double r
    /*, bool segment */ )
{
    double x0 = cp.x, y0 = cp.y;
    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double A = y2 - y1;
    double B = x1 - x2;
    double C = x2 * y1 - x1 * y2;
    double a = sq(A) + sq(B);
    double b, c, d;
//    bool bnz = true;
    //int cnt = 0;

    if (fabs(B) >= eps) {
        // if B isn't zero or close to it
        b = 2 * (A * C + A * B * y0 - sq(B) * x0);
        c = sq(C) + 2 * B * C * y0 - sq(B) * (sq(r) - sq(x0) - sq(y0));
    } else {
        b = 2 * (B * C + A * B * x0 - sq(A) * y0);
        c = sq(C) + 2 * A * C * x0 - sq(A) * (sq(r) - sq(x0) - sq(y0));
//        bnz = false;
    }
    d = sq(b) - 4 * a * c; // discriminant
    if (d < 0) {
        // line & circle don't intersect
        //printf("[]\n");
        return false;
    }

#if 0
    if (d == 0) {
        // line is tangent to circle, so just one intersect at most
        if (bnz) {
            double x = -b / (2 * a);
            double y = fx(A, B, C, x);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
        } else {
            double y = -b / (2 * a);
            double x = fy(A, B, C, y);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
        }
    } else {
        // two intersects at most
        d = sqrt(d);
        if (bnz) {
            double x = (-b + d) / (2 * a);
            double y = fx(A, B, C, x);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);

            x = (-b - d) / (2 * a);
            y = fx(A, B, C, x);
            cnt += rxy(x1, y1, x2, y2, x, y, segment);
        } else {
            double y = (-b + d) / (2 * a);
            double x = fy(A, B, C, y);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);

            y = (-b - d) / (2 * a);
            x = fy(A, B, C, y);
            cnt += rxy(x1, y1, x2, y2, x, y, segment);
        }
    }
#endif
    return true;

//    if (cnt <= 0) {
//        printf("[]");
//    }
}

int RectDiagonalLength(int w, int h)
{
    return (int)sqrt(w * w + h * h);
}

Box BoxFromRect(SDL_Rect rect)
{
    return (Box){ rect.x, rect.y, rect.x + rect.h, rect.y + rect.h };
}

// https://noonat.github.io/intersect/

bool RectInRect(SDL_Rect a, SDL_Rect b)
{
    int right[2]    = { a.x, b.x };
    int left[2]     = { a.x + a.w, b.x + b.w };
    int top[2]      = { a.y, b.y };
    int bottom[2]   = { a.y + a.h, b.y + b.h };

    return
       right[0] >= right[1]
    && left[0] <= left[1]
    && top[0] >= top[1]
    && bottom[0] <= bottom[1];
}

bool RectsIntersect(SDL_Rect a, SDL_Rect b)
{
    return
    (abs((a.x + a.w / 2) - (b.x + b.w / 2)) * 2 < (a.w + b.w)) &&
    (abs((a.y + a.h / 2) - (b.y + b.h / 2)) * 2 < (a.h + b.h));
}

bool LinesIntersect
(   vec2_t a1, vec2_t a2,
    vec2_t b1, vec2_t b2,
    vec2_t * intersection)
{
    float adx = a2.x - a1.x;
    float ady = a2.y - a1.y;
    float bdx = b2.x - b1.x;
    float bdy = b2.y - b1.y;
    float cdx = a1.x - b1.x;
    float cdy = a1.y - b1.y;

    float s = (-ady * cdx + adx * cdy) / (-bdx * ady + adx * bdy);
    float t = ( bdx * cdy - bdy * cdx) / (-bdx * ady + adx * bdy);

    if ( s >= 0 && s <= 1 && t >= 0 && t <= 1 ) {
        if ( intersection != NULL ) {
            intersection->x = a1.x + (t * adx);
            intersection->y = a1.y + (t * ady);
        }
        return true;
    }

    return false;
}

static void GetRectSides(SDL_Rect r, vec2_t sides[4][2])
{
    sides[RECT_SIDE_TOP][0]     = (vec2_t){ r.x, r.y };
    sides[RECT_SIDE_TOP][1]     = (vec2_t){ r.x + r.w, r.y };
    sides[RECT_SIDE_BOTTOM][0]  = (vec2_t){ r.x, r.y + r.h };
    sides[RECT_SIDE_BOTTOM][1]  = (vec2_t){ r.x + r.w, r.y + r.h };
    sides[RECT_SIDE_LEFT][0]    = sides[RECT_SIDE_TOP][0];
    sides[RECT_SIDE_LEFT][1]    = sides[RECT_SIDE_BOTTOM][0];
    sides[RECT_SIDE_RIGHT][0]   = sides[RECT_SIDE_TOP][1];
    sides[RECT_SIDE_RIGHT][1]   = sides[RECT_SIDE_BOTTOM][1];
}

rect_side_t RectSideLineIntersection(vec2_t point, SDL_Rect rect)
{
    vec2_t rect_center = { rect.x + rect.w / 2, rect.y + rect.h / 2 };
    vec2_t sides[4][2];
    GetRectSides(rect, sides);

    for ( rect_side_t side = RECT_SIDE_TOP; side < NUM_RECT_SIDES; side++ ) {
        for ( int i = 0; i < NUM_RECT_SIDES; i++ ) {
            bool intersected = LinesIntersect
            (   point,
                rect_center,
                sides[side][0],
                sides[side][1],
                NULL );

            if ( intersected ) {
                return side;
            }
        }
    }

    return RECT_SIDE_NONE;
}

// TODO: translate
//function getRandomPointInCircle(radius)
//  local t = 2*math.pi*math.random()
//  local u = math.random()+math.random()
//  local r = nil
//  if u > 1 then r = 2-u else r = u end
//  return radius*r*math.cos(t), radius*r*math.sin(t)
//end

bool PointInRect(int x, int y, SDL_Rect r)
{
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

bool RectsOverlap(SDL_Rect a, SDL_Rect b)
{
    int corner_xs[4] = { a.x, a.x + a.w - 1, a.x, a.x + a.w - 1 };
    int corner_ys[4] = { a.y, a.y, a.y + a.h - 1, a.y + a.h - 1 };

    for ( int i = 0; i < 4; i++ ) {
        if ( PointInRect(corner_xs[i], corner_ys[i], b) ) {
            return true;
        }
    }

    return false;
}

int ManhattanDistance(int x1, int y1, int x2, int y2)
{
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return dx + dy;
}


#pragma mark - RANDOM

static u32 next = 1;

void SeedRandom(u32 seed)
{
    next = seed;
}

void Randomize(void)
{
    next = (u32)time(NULL);
}

// https://lemire.me/blog/

static inline u32 Wyhash32(void)
{
    uint64_t tmp;
    uint32_t m1, m2;

    next += 0xE120FC15;
    tmp  = (uint64_t)next * 0x4A39B70D;
    m1   = (uint32_t)(( tmp >> 32) ^ tmp );
    tmp  = (uint64_t)m1 * 0x12FAD5C9;
    m2   = (uint32_t)( (tmp >> 32) ^ tmp );

    return m2;
}

u32 Random32(void)
{
    return Wyhash32();
}

u32 Random(u32 min, u32 max)
{
    return Wyhash32() % (max - min + 1) + min;
}

static inline float _RandomFloat(void)
{
    return (float)((double)Wyhash32() / (double)RANDOM_MAX);
}

float RandomFloat(float min, float max)
{
    return _RandomFloat() * (max - min) + min;
}

void WrapPosition(vec2_t * position, int w, int h)
{
    position->x = fmod(position->x + w, w);
    position->y = fmod(position->y + h, h);
}

vec2_t RandomDirection(void)
{
    return Vec2Rotate((vec2_t){ 1, 0 }, DEG2RAD(Random(0, 359)));
}

vec2_t RandomVelocity(float min, float max)
{
    vec2_t v = RandomDirection();
    v = Vec2Scale(v, RandomFloat(min, max));

    return v;
}

void RandomizeVector(vec2_t * v, float radians) {
    *v =  Vec2Rotate(*v, RandomFloat(-radians, +radians));
}

bool Chance(float percent)
{
    return _RandomFloat() < percent;
}

#pragma mark - NOISE

#define P_SIZE 512

//
// These values always are copied to `p` when randomizing noise. This way
// the same seed always produces the same result.
//
static const uint8_t originalPermutation[P_SIZE] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
    230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
    1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
    116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
    124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
    47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
    154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
    108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
    242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
    239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121,
    50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243,
    141, 128, 195, 78, 66, 215, 61, 156, 180,
    // -------------------------------------------------------------------------
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
    230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
    1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
    116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
    124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
    47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
    154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
    108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
    242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
    239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121,
    50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243,
    141, 128, 195, 78, 66, 215, 61, 156, 180,
};

static uint8_t p[P_SIZE] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
    230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
    1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
    116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
    124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
    47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
    154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
    108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
    242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
    239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121,
    50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243,
    141, 128, 195, 78, 66, 215, 61, 156, 180,
    // -------------------------------------------------------------------------
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
    230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
    1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
    116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
    124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
    47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
    154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
    108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
    242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
    239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121,
    50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243,
    141, 128, 195, 78, 66, 215, 61, 156, 180,
};

static float fade(float t)
{
    return t*t*t*(t*(t*6 - 15) + 10);
}

static float lerp(float t, float a, float b)
{
    return a + t*(b - a);
}

static float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h<8 ? x : y,
           v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

//
// COPYRIGHT 2002 KEN PERLIN
// Adapted from Java code by Thomas Foster
//

static float perlin(float x, float y, float z)
{
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);
    int A = p[X    ] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
    int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA    ], x  , y  , z ),
                                   grad(p[BA    ], x-1, y  , z )),
                           lerp(u, grad(p[AB    ], x  , y-1, z ),
                                   grad(p[BB    ], x-1, y-1, z ))),
                   lerp(v, lerp(u, grad(p[AA + 1], x  , y  , z-1 ),
                                   grad(p[BA + 1], x-1, y  , z-1 )),
                           lerp(u, grad(p[AB + 1], x  , y-1, z-1 ),
                                   grad(p[BB + 1], x-1, y-1, z-1 ))));

}

void RandomizeNoise(u32 seed)
{
    memcpy(p, originalPermutation, sizeof(p)); // restart
    SeedRandom(seed);

    // shuffle
    for ( int i = 0; i < P_SIZE; i++) {
        int r = Random(0, P_SIZE - 1);

        uint8_t temp = p[i];
        p[i] = p[r];
        p[r] = temp;
    }
}

float Noise2
(   float x,
    float y,
    float z,
    float frequency,
    int   octaves,
    float amplitude,
    float persistence,
    float lacunarity )
{
    float total = 0;

    for ( int i = 0; i < octaves; i++ ) {
        total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return total;
}

extern inline float Noise(float x, float y, float z);


/*

 noise that wraps around:

 Use this: https://gist.github.com/KdotJPG/f4db4491b341b8987f4a and ignore the 3rd coordinate.
 It's not C# so you'll have to port it (shouldn't be too hard), but it is a tileable implementation I made a while back.
 Note that the tileability technique used here is specific to 3D OpenSimplex, because it uses a rational lattice. It doesn't work for 2D or 4D.
 This is probably the most "normal" way to do this. Just note that it only tiles in multiples of 6, because that's when the lattice lines back up with itself.
 EDIT: Slight chance there's a bug that occurs when x, y, and z don't all tile with the same period. I haven't extensively tested this, I just made it as a quick solution once.
 Alternatively, you can use this trick with 3D noise:
 noise(
     y / FEATURE_SIZE,
     sin(x * 2pi / WIDTH) / 2pi * WIDTH / FEATURE_SIZE,
     cos(x * 2pi / WIDTH) / 2pi * WIDTH / FEATURE_SIZE
 );
 Imagine this as wrapping around a cylinder, where the height equals the circumference.
 Note: This results in a different "character" as you change in the X coordinate versus the Y coordinate. But depending on your use, that may either be good thing or a bad thing.
 EDIT:
 FEATURE_SIZE gets bigger as you want things in your terrain to get bigger.
 Inside sin/cos, we multiply by 2pi and divide by WIDTH so that sin/cos roll over exactly once we've crossed over.
 outside sin/cos, we divide by 2pi because the circumference is 2pi (we want to deal with multiples of the distance "one"), then we multiply by (WIDTH / FEATURE_SIZE) which is the number of times you want to go a distance of "one" in your noise over the entire width.
 Also this generalizes straight to the 4D torus wrapping technique if you do the same thing for y.
 There is also the interpolation trick mentioned a bit down. That works and it's easy to understand, but I generally recommend against it if you have the choice, because when you linearly interpolate two noise values, the average absolute value of the noise becomes less, and you will more often see taller hills towards the edges where you're predominantly seeing one noise function. You can technically try to come up with a function to equalize these differences, but keep in mind that it's not just the height that's changing, but the probability distribution of different heights.

 */
