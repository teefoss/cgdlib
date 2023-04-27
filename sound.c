//
//  sound.c
//  RogueLike
//
//  Created by Thomas Foster on 11/8/22.
//

#include "sound.h"

#include <SDL.h>

static SDL_AudioSpec spec;
static SDL_AudioDeviceID device;
static uint8_t beeper_volume = 8;

static double NoteNumberToFrequency(int note_num)
{
    if ( note_num == 0 ) {
        return 0;
    }

    static const int freqiencies[] = { // in octave 6
        4186, // C
        4435, // C#
        4699, // D
        4978, // D#
        5274, // E
        5588, // F
        5920, // F#
        6272, // G
        6645, // G#
        7040, // A
        7459, // A#
        7902, // B
    };

    int octave = (note_num - 1) / 12;
    int note = (note_num - 1) % 12;
    int freq = freqiencies[note];

    int octaves_down = 6 - octave;
    while ( octaves_down-- )
        freq /= 2;

    return (double)freq * 2.0; // basic notes sound 1 octave higher
}

static void QueueNoteNumber(int note_num, int note_ms, int silence_ms)
{
    QueueSound(NoteNumberToFrequency(note_num), note_ms);
    QueueSound(0, silence_ms);
}

void QueueSound(unsigned frequency, unsigned milliseconds)
{
    float period = (float)spec.freq / (float)frequency;
    int len = (float)spec.freq * ((float)milliseconds / 1000.0f);

    for ( int i = 0; i < len; i++ ) {
        int8_t sample;

        if ( frequency == 0 ) {
            sample = spec.silence;
        }
        else if ( frequency & NOISE && arc4random_uniform(256) == 0 ) {
            sample = arc4random_uniform(2) == 0 ? beeper_volume : -beeper_volume;
        }
        else {
            sample = (int)((float)i / period) % 2 ? beeper_volume : -beeper_volume;
        }

        SDL_QueueAudio(device, &sample, sizeof(sample));
    }
}

// play whatever is in the queue.
static int PlayQueuedSound(void * ptr)
{
    (void)ptr; // present so signature matches type SDL_ThreadFunction

    SDL_PauseAudioDevice(device, SDL_FALSE);

    while ( SDL_GetQueuedAudioSize(device) )
        ;

    SDL_PauseAudioDevice(device, SDL_TRUE);

    return 0;
}

void PlayQueuedSoundAsync(void)
{
    SDL_Thread * thread;
    thread = SDL_CreateThread(PlayQueuedSound, "play_thread", NULL);
    if ( thread == NULL ) {
        fprintf(stderr, "could not create play thread: %s\n", SDL_GetError());
    } else {
        SDL_DetachThread(thread);
    }
}

static void S_ShutdownSound(void)
{
    SDL_CloseAudioDevice(device);
}

void S_InitSound(void)
{
    if ( SDL_WasInit(SDL_INIT_AUDIO) == 0 ) {
        int result = SDL_InitSubSystem(SDL_INIT_AUDIO);
        if ( result < 0 )
            fprintf(stderr, "error: failed to init SDL audio subsystem: %s", SDL_GetError());
    }

    SDL_AudioSpec want = {
        .freq = 44100,
        .format = AUDIO_S8,
        .channels = 1,
        .samples = 4096,
    };

    device = SDL_OpenAudioDevice(NULL, 0, &want, &spec, 0);
    if ( device == 0 ) {
        fprintf(stderr, "error: failed to open audio: %s\n", SDL_GetError());
    }

    atexit(S_ShutdownSound);
}

void Volume(unsigned value)
{
    if ( value > 15 ) {
        fprintf(stderr, "bad volume\n");
        return;
    }

    beeper_volume = value;
}

void S_Sound(unsigned frequency, unsigned milliseconds)
{
    SDL_ClearQueuedAudio(device);
    QueueSound(frequency, milliseconds);
    PlayQueuedSound(NULL);
}

void SoundAsync(unsigned frequency, unsigned milliseconds)
{
    QueueSound(frequency, milliseconds);
    PlayQueuedSoundAsync();
}

void StopSoundAsync(void)
{
    SDL_ClearQueuedAudio(device);
    SDL_PauseAudioDevice(device, SDL_TRUE);
}

void Beep(void)
{
    S_Sound(800, 200);
}

#define PLAY_DEBUG 0

static void PlayError(const char * msg, int line_position)
{
    printf("Play syntax error: %s (position %d)\n.", msg, line_position);
}

void S_Play(const char * string, ...)
{
    va_list args[2];
    va_start(args[0], string);
    va_copy(args[1], args[0]);

    int str_len = vsnprintf(NULL, 0, string, args[0]);
    if ( str_len > 1024 ) {
        return PlayError("play string too long!", 0);
    }

    char * buffer = alloca(str_len + 1);
    vsnprintf(buffer, str_len + 1, string, args[1]);
    va_end(args[0]);
    va_end(args[1]);

    // default settings
    int bmp = 120;
    int oct = 4;
    int len = 4;
    int background = 1;

    static const int note_offsets[7] = { 9, 11, 0, 2, 4, 5, 7 };

    enum {
        mode_staccato = 6,  // 6/8
        mode_normal = 7,    // 7/8
        mode_legato = 8     // 8/8
    } mode = mode_normal;

    SDL_ClearQueuedAudio(device);

    // queue up whatever's in the string:

    const char * str = buffer;
    while ( *str != '\0') {
        char c = toupper(*str++);
        switch ( c ) {
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'N': case 'P':
            {
                // get note:
                int note = 0;
                switch ( c ) {
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
                        note = 1 + (oct) * 12 + note_offsets[c - 'A'];
                        break;
                    case 'P':
                        note = 0;
                        break;
                    case 'N': {
                        int number = (int)strtol(str, (char **)&str, 10);
                        if ( number < 0 || number > 84 )
                            return PlayError("bad note number", (int)(str - string));
                        if ( number > 0 )
                            note = number;
                        break;
                    }
                    default:
                        break;
                }

                // adjust note per accidental:
                if ( c >= 'A' && c <= 'G' ) {
                    if ( *str == '+' || *str == '#' ) {
                        if ( note < 84 )
                            note++;
                        str++;
                    } else if ( *str == '-' ) {
                        if ( note > 1 )
                            note--;
                        str++;
                    }
                }

                int d = len;

                // get note value:
                if ( c != 'N' ) {
                    int number = (int)strtol(str, (char **)&str, 10);
                    if ( number < 0 || number > 64 )
                        return PlayError("bad note value", (int)(str - string));
                    if ( number > 0 )
                        d = number;
                }

                // count dots:
                int dot_count = 0;
                while ( *str == '.' ) {
                    dot_count++;
                    str++;
                }

                // adjust duration if there are dots:
                float total_ms = (60.0f / (float)bmp) * 1000.0f * (4.0f / (float)d);
                float prolongation = total_ms / 2.0f;
                while ( dot_count-- ) {
                    total_ms += prolongation;
                    prolongation /= 2;
                }

                // calculate articulation silence:
                int note_ms = total_ms * ((float)mode / 8.0f);
                int silence_ms = total_ms * ((8.0f - (float)mode) / 8.0f);

                // and finally, queue it
                QueueNoteNumber(note, note_ms, silence_ms);
                break;
            } // A-G, N, and P

            case 'T':
                bmp = (int)strtol(str, (char **)&str, 10);
                if ( bmp == 0 )
                    return PlayError("bad tempo", (int)(str - string));
                #if PLAY_DEBUG
                printf("set tempo to %d\n", bmp);
                #endif
                break;

            case 'O':
                if ( *str < '0' || *str > '6' )
                    return PlayError("bad octave", (int)(str - string));
                oct = (int)strtol(str, (char **)&str, 10);
                #if PLAY_DEBUG
                printf("set octave to %d\n", oct);
                #endif
                break;

            case 'L':
                len = (int)strtol(str, (char **)&str, 10);
                if ( len < 1 || len > 64 )
                    return PlayError("bad length", (int)(str - string));
                #if PLAY_DEBUG
                printf("set length to %d\n", len);
                #endif
                break;

            case '>':
                if ( oct < 6 )
                    oct++;
                #if PLAY_DEBUG
                printf("increase octave\n");
                #endif
                break;

            case '<':
                if ( oct > 0 )
                    oct--;
                #if PLAY_DEBUG
                printf("decrease octave\n");
                #endif
                break;

            case 'M': {
                char option = toupper(*str++);
                switch ( option ) {
                    case 'L': mode = mode_legato; break;
                    case 'N': mode = mode_normal; break;
                    case 'S': mode = mode_staccato; break;
                    case 'B': background = 1; break;
                    case 'F': background = 0; break;
                    default:
                        return PlayError("bad music option", (int)(str - string));
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    if ( background ) {
        PlayQueuedSoundAsync();
    } else {
        PlayQueuedSound(NULL);
    }
}

#undef PLAY_DEBUG
