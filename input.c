//
//  input.c
//  Game
//
//  Created by Thomas Foster on 9/27/22.
//

#include "input.h"
#include "mathlib.h"

#define MAX_VALUE       30000.0f
#define MIN_VALUE_1D    250.0f // triggers
#define MIN_VALUE_2D    2500.0f // sticks

typedef struct {
    const u8 * curr_keys;
    u8 prev_keys[SDL_NUM_SCANCODES];
} keyboard_state_t;

typedef struct {
    u8 curr_buttons[SDL_CONTROLLER_BUTTON_MAX];
    u8 prev_buttons[SDL_CONTROLLER_BUTTON_MAX];
    float triggers[2];
    vec2_t sticks[2];
} controller_state_t;

typedef struct {
    u32 curr_buttons;
    u32 prev_buttons;
    vec2_t position;
    // TODO: scroll wheel
} mouse_state_t;

struct input_state {
    keyboard_state_t keyboard_state;
    controller_state_t controller_state;
    mouse_state_t mouse_state;

    SDL_GameController * controller;
};

#pragma mark - PRIVATE

static float Filter1D(s16 value)
{
    CLAMP(value, MIN_VALUE_1D, MAX_VALUE);
    float result = MAP(value, MIN_VALUE_1D, MAX_VALUE, 0.0f, 1.0f);

    return result;
}

static vec2_t Filter2D(s16 x, s16 y)
{
    vec2_t vec = { (float)x, (float)y };

    float length = Vec2Length(vec);

    if ( length < MIN_VALUE_2D ) {
        vec = (vec2_t){ 0.0f, 0.0f };
    } else {
        // Calculate fractional value between dead zone and max value
        float f = (length - MIN_VALUE_2D) / (MAX_VALUE - MIN_VALUE_2D);

        CLAMP(f, 0.0f, 1.0f);

        vec = Vec2Scale(vec, f / length);
    }

    return vec;
}

#pragma mark - PUBLIC

input_state_t * IN_Initialize(void)
{
    if ( !SDL_WasInit(SDL_INIT_GAMECONTROLLER) ) {
        if ( SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0 ) {
            Error("could not initialize SDL game controller subsystem");
        }
    }

    input_state_t * state = calloc(1, sizeof(*state));
    if ( state == NULL ) {
        Error("could not allocate input state");
    }

    state->keyboard_state.curr_keys = SDL_GetKeyboardState(NULL);

    state->controller = SDL_GameControllerOpen(0);

    return state;
}

void IN_StartFrame(input_state_t * state)
{
    memcpy
    (   state->keyboard_state.prev_keys,
        state->keyboard_state.curr_keys,
        sizeof(state->keyboard_state.prev_keys) );

    memcpy
    (   state->controller_state.prev_buttons,
        state->controller_state.curr_buttons,
        sizeof(state->controller_state.prev_buttons) );

    state->mouse_state.prev_buttons = state->mouse_state.curr_buttons;
}

void IN_ProcessEvent(input_state_t * state, SDL_Event event)
{
    switch ( event.type ) {
        case SDL_CONTROLLERDEVICEADDED:
            if ( state->controller == NULL ) {
                state->controller = SDL_GameControllerOpen(0);
                puts("controller connected");
            }
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            if ( state->controller != NULL ) {
                SDL_GameControllerClose(state->controller);
                state->controller = NULL;
                puts("controller disconnected");
            }
            break;
        default:
            break;
    }
}

void IN_Update(input_state_t * state)
{
    controller_state_t * cs = &state->controller_state;

    // Controller buttons

    for ( int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++ ) {
        cs->curr_buttons[i] = SDL_GameControllerGetButton(state->controller, i);
    }

    // Controller triggers

    s16 v;
    v = SDL_GameControllerGetAxis(state->controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    cs->triggers[SIDE_LEFT] = Filter1D(v);
    v = SDL_GameControllerGetAxis(state->controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    cs->triggers[SIDE_RIGHT] = Filter1D(v);

    // Controller left stick

    s16 x = SDL_GameControllerGetAxis(state->controller, SDL_CONTROLLER_AXIS_LEFTX);
    s16 y = SDL_GameControllerGetAxis(state->controller, SDL_CONTROLLER_AXIS_LEFTY);
    cs->sticks[SIDE_LEFT] = Filter2D(x, y);

    // Controller right stick

    x = SDL_GameControllerGetAxis(state->controller, SDL_CONTROLLER_AXIS_RIGHTX);
    y = SDL_GameControllerGetAxis(state->controller, SDL_CONTROLLER_AXIS_RIGHTY);
    cs->sticks[SIDE_RIGHT] = Filter2D(x, y);

    // Mouse

    int mouse_x = 0;
    int mouse_y = 0;
    state->mouse_state.curr_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    state->mouse_state.position = (vec2_t){ mouse_x, mouse_y };
}

#pragma mark - KEYBOARD

bool IN_IsKeyDown(input_state_t * state, SDL_Scancode code)
{
    return state->keyboard_state.curr_keys[code] == 1;
}

button_state_t IN_GetKeyState(input_state_t * state, SDL_Scancode code)
{
    if ( state->keyboard_state.prev_keys[code] ) {
        return state->keyboard_state.curr_keys[code]
        ? IN_HELD
        : IN_RELEASED;
    } else {
        return state->keyboard_state.curr_keys[code]
        ? IN_PRESSED
        : IN_NONE;
    }
}

#pragma mark - CONTROLLER

bool IN_IsControllerConnected(input_state_t * state)
{
    return state->controller != NULL;
}

bool IN_IsControllerButtonDown(input_state_t * state, SDL_GameControllerButton button)
{
    return state->controller_state.curr_buttons[button] == 1;
}

button_state_t IN_GetControllerButtonState(input_state_t * state, SDL_GameControllerButton button)
{
    if ( state->controller_state.prev_buttons[button] ) {
        return state->controller_state.curr_buttons[button]
        ? IN_HELD
        : IN_RELEASED;
    } else {
        return state->controller_state.curr_buttons[button]
        ? IN_PRESSED
        : IN_NONE;
    }
}

vec2_t IN_GetStickDirection(input_state_t * state, controller_side_t side)
{
    if ( IN_IsControllerConnected(state) ) {
        return state->controller_state.sticks[side];
    } else {
        return (vec2_t){ 0 };
    }
}

float IN_GetTriggerState(input_state_t * state, controller_side_t side)
{
    if ( IN_IsControllerConnected(state) ) {
        return state->controller_state.triggers[side];
    } else {
        return 0.0f;
    }
}

#pragma mark - MOUSE

button_state_t IN_GetMouseButtonState(input_state_t * state, int button)
{
    if ( state->mouse_state.prev_buttons & button ) {
        return state->mouse_state.curr_buttons & button
        ? IN_HELD
        : IN_RELEASED;
    } else {
        return state->mouse_state.curr_buttons & button
        ? IN_PRESSED
        : IN_NONE;
    }
}

bool IN_IsMouseButtonDown(input_state_t * state, int button)
{
    return state->mouse_state.curr_buttons & button;
}

vec2_t IN_GetMousePosition(input_state_t * state)
{
    return state->mouse_state.position;
}
