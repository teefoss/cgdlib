//
//  vector.c
//  Game
//
//  Created by Thomas Foster on 9/22/22.
//

#include "vector.h"
#include "mathlib.h"

const vec2_t vec2_zero = { 0.0f, 0.0f };
const vec3_t vec3_zero = { 0.0f, 0.0f, 0.0f };

extern inline vec2_t Vec2Add(vec2_t a, vec2_t b);
extern inline vec2_t Vec2Scale(vec2_t v, float s);
extern inline vec2_t Vec2Subtract(vec2_t a, vec2_t b);
extern inline float Vec2LengthSqr(vec2_t v);
extern inline float Vec2Length(vec2_t);
extern inline float Vec2Angle(vec2_t v);

vec2_t Vec2Lerp(const vec2_t * v, const vec2_t * target, float w)
{
    vec2_t result;
    result.x = Lerp(v->x, target->x, w);
    result.y = Lerp(v->y, target->y, w);

    return result;
}

vec2_t Vec2LerpEpsilon(vec2_t v, vec2_t target, float w, float epsilson)
{
    vec2_t result;
    result.x = Lerp(v.x, target.x, w);
    result.y = Lerp(v.y, target.y, w);

    if ( fabsf(result.x - target.x) < epsilson ) {
        result.x = target.x;
    }

    if ( fabsf(result.y - target.y) < epsilson ) {
        result.y = target.y;
    }

    return result;
}

vec3_t Vec3Lerp(const vec3_t * v, const vec3_t * target, float w)
{
    vec3_t result;
    result.x = Lerp(v->x, target->x, w);
    result.y = Lerp(v->y, target->y, w);
    result.z = Lerp(v->z, target->z, w);

    return result;
}

vec2_t Vec2Normalize(vec2_t v)
{
    vec2_t result = { 0, 0 };

    float length = sqrtf(v.x * v.x + v.y * v.y);
    if ( length == 0.0f ) {
        return result;
    }

    float ilength = 1.0 / length;
    result.x = v.x * ilength;
    result.y = v.y * ilength;

    return result;
}

vec2_t Vec2Rotate(vec2_t v, float radians)
{
    vec2_t result;
    result.x = cos(radians) * v.x - sin(radians) * v.y;
    result.y = sin(radians) * v.x + cos(radians) * v.y;

    return result;
}
