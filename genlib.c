#include "genlib.h"
#include <dirent.h> // TODO: check if this is portable

#define PRINT_DEF(func, T, format) \
    PRINT_DECL(func, T) { printf("%s: "format"\n", name, value); }

PRINT_DEF(print_uchar, unsigned char, "%d")
PRINT_DEF(print_int, int, "%d")
PRINT_DEF(print_float, float, "%f")
PRINT_DEF(print_double, double, "%f")
PRINT_DEF(print_ushort, unsigned short, "%d")
PRINT_DEF(print_ulong, unsigned long, "%lu")

void print_sdl_point(const char * name, SDL_Point point)
{
    printf("%s: (%d, %d)\n", name, point.x, point.y);
}

void print_sdl_rect(const char * name, SDL_Rect r)
{
    printf("%s: x: %d, y: %d, w: %d, h: %d\n", name, r.x, r.y, r.w, r.h);
}

const char * GetExtension(const char * path)
{
    const char * dot = strrchr(path, '.'); // find the last dot
    if ( dot == NULL || dot == path ) {
        return "";
    }

    return dot + 1;
}

void IntToBinaryString(int integer, int num_bits, char * out)
{
    for ( int bit = num_bits - 1; bit >= 0; bit--) {
        *out++ = integer & (1 << bit) ? '1' : '0';
    }
}

FILE * OpenFile(const char * file_name, const char * mode)
{
    FILE * file = fopen(file_name, mode);

    if ( file == NULL ) {
        Error("could not open %s", file_name);
    }

    return file;
}

// TODO: check portability
float ProgramTime(void)
{
    struct timeval timeval;
    static long seconds = 0;

    gettimeofday(&timeval, NULL);
    if ( seconds == 0 ) {
        seconds = timeval.tv_sec;
        return timeval.tv_usec / 1000000.0f;
    }

    return (timeval.tv_sec - seconds) + timeval.tv_usec / 1000000.0f;
}

unsigned StringHash(const char * key)
{
    /*
     Good hash table prime numbers from
     https://planetmath.org/goodhashtableprimes

     Use table size = num table entries * 1.3

     193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
     196613, 393241, 786433, 1572869, 3145739, 6291469,
     12582917, 25165843, 50331653, 100663319, 201326611,
     402653189, 805306457, 1610612741,
     */

    // djb2 string hash function:
    unsigned result = 5381;
    const char * p = key;

    while ( *p != '\0' ) {
        result = (result << 5) + result + *p;
        ++p;
    }

    return result;
}

// TODO: test this
int GetAllFilesInDirectory(const char * path, const char * file_ext, char ** out)
{
    DIR * dir = opendir(path);
    if ( dir == NULL ) {
        Error("could not open directory '%s'", path);
    }

    int num_files = 0;
    struct dirent * entry;
    while (( entry = readdir(dir) )) {
        const char * file = entry->d_name;

        // skip the '.' and '..'
        if ( strcmp(".", file) == 0 || strcmp("..", file) == 0 ) {
            continue;
        }

        // skip files with non-matching extension
        const char * this_ext = GetExtension(file);
        if ( file_ext != NULL && strcmp(this_ext, file_ext) != 0 ) {
            continue;
        }

        num_files++;
        out[num_files++] = SDL_strdup(file);
    }

    return num_files;
}
