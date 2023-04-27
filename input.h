//
//  input.h
//  Game
//
//  Created by Thomas Foster on 9/27/22.
//

#ifndef input2_h
#define input2_h

#include "genlib.h"
#include "vector.h"

typedef enum {
    IN_UNDEFINED = -1,
    IN_NONE,
    IN_PRESSED, // Button/key went down this frame.
    IN_HELD,
    IN_RELEASED // Button/key was released this frame.
} button_state_t; // also for keys

typedef enum {
    SIDE_LEFT,
    SIDE_RIGHT,
} controller_side_t;

typedef struct input_state input_state_t;

input_state_t * IN_Initialize(void);
void IN_StartFrame(input_state_t *);
void IN_ProcessEvent(input_state_t *, SDL_Event event);
void IN_Update(input_state_t *);

// Keyboard

bool IN_IsKeyDown(input_state_t *, SDL_Scancode code);
button_state_t IN_GetKeyState(input_state_t *, SDL_Scancode code);

// Controller

bool IN_IsControllerConnected(input_state_t *);
bool IN_IsControllerButtonDown(input_state_t *, SDL_GameControllerButton button);
button_state_t IN_GetControllerButtonState(input_state_t *, SDL_GameControllerButton button);
vec2_t IN_GetStickDirection(input_state_t *, controller_side_t side);
float IN_GetTriggerState(input_state_t *, controller_side_t side);

// Mouse

bool IN_IsMouseButtonDown(input_state_t *, int button);
button_state_t IN_GetMouseButtonState(input_state_t * state, int button);
vec2_t IN_GetMousePosition(input_state_t *);

#endif /* input2_h */
