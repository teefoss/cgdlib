//
//  sound.h
//  RogueLike
//
//  Created by Thomas Foster on 11/8/22.
//

#ifndef sound_h
#define sound_h

#define NOISE 0x8000

/// Call to initialize sound.
void S_InitSound(void);

// play a frequency for given duration
// suspends program execution while sound plays
void S_Sound(unsigned frequency, unsigned milliseconds);

// play a frequency for given duration asynchronously
void SoundAsync(unsigned frequency, unsigned milliseconds);

// stop any sound being played asynchronously
void StopSoundAsync(void);

// play a note of 800 Hz for 0.2 seconds
void Beep(void);

/// Play a BASIC-style music string.
///
/// Format:
/// L[1,2,4,8,16,32,64] (default: 4)
/// O[0...6] (default: 4)
/// T[32...255] (default: 120)
/// [A...G]([+,#,-][1,2,4,8,16,32,64][.])
/// N[0...84](.)
/// P[v]
void S_Play(const char * string, ...);

/// Queue up a sound in the audio buffer.
void QueueSound(unsigned frequency, unsigned milliseconds);

/// Play whatever is in the audio buffer (put there previously with a call
/// to `QueueSound`).
void PlayQueuedSoundAsync(void);


#endif /* sound_h */
