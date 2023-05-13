// -----------------------------------------------------------------------------
// Texture loading, storage and retrieval.
// -----------------------------------------------------------------------------
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL.h>

/// In `directory`, load all image files of given extension.
///
/// The BPM transparency color is assumed to be #FF00FF.
/// - Parameter directoryName: directory (relative to cwd) containing
///   files to be loaded
/// - Parameter file_extension: the file extention (without the dot) of the
///   files in `directory`, for example, "bmp" or "png".
void LoadTextures(const char * directory_name, const char * file_extension);

/// Get texture for given key.
///
/// - Parameter key: The file name of the texture.
/// - Returns: The requested texture. If no texture is found for given key,
///   the program is terminated via `Error()`.
SDL_Texture * GetTexture(const char * key);

SDL_Rect GetScaledTextureSize(SDL_Texture * texture, int draw_scale);

void FreeAllTextures(void);
void PrintTextureHashTable(void);

#endif /* __TEXTURE_H__ */
