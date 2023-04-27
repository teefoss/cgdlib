//
//  texture.c
//  Space Invaders
//
//  Created by Thomas Foster on 8/26/22.
//

#include "texture.h"
#include "genlib.h"
#include "video.h"

#include <SDL_image.h>

// use size of 1.3 x number of textures
#define HASH_TABLE_SIZE 193

typedef struct texture_node texture_node_t;
struct texture_node {
    char * key;
    SDL_Texture * texture;
    texture_node_t * next;
};

static texture_node_t * texture_table[HASH_TABLE_SIZE];

SDL_Texture * GetTexture(const char * name)
{
    static int total_textures = 0;
    unsigned index = StringHash(name) % HASH_TABLE_SIZE;

    // Find the texture.
    texture_node_t * entry = texture_table[index];
    while ( entry ) {
        if ( strcmp(name, entry->key) == 0 ) {
            return entry->texture;
        }
        entry = entry->next;
    }

    // Texture not found, load it.
    SDL_Texture * texture = NULL;
    SDL_Surface * surface = IMG_Load(name);
    if ( surface != NULL ) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    if ( texture ) {
        texture_node_t * node = malloc(sizeof(*node));
        if ( node == NULL ) {
            Error("could not allocate texture hash table node");
        }

        // Create a new node and insert it into hash table.
        node->key = SDL_strdup(name);
        node->texture = texture;
        node->next = texture_table[index];
        texture_table[index] = node;

        total_textures++;
        printf("%3d: loaded %s\n", total_textures, name);
        if ( (float)total_textures * 1.3f > (float)HASH_TABLE_SIZE ) {
            puts("please increase texture hash table size!\n");
        }
    } else {
        Error("Could not load %s", name);
    }

    return texture;
}

// debug
void PrintTextureHashTable(void)
{
    for ( int i = 0; i < HASH_TABLE_SIZE; i++ ) {
        printf("table entry %3d: ", i);
        if ( texture_table[i] == NULL ) {
            puts("NULL");
        } else {
            texture_node_t * node = texture_table[i];
            while ( node ) {
                printf("\n - %s", node->key);
                node = node->next;
            }
            printf("\n");
        }
    }
}

void FreeAllTextures(void)
{
    for ( int i = 0; i < HASH_TABLE_SIZE; i++ ) {
        texture_node_t * node = texture_table[i];

        while ( node ) {
            texture_node_t * next = node->next;

            SDL_DestroyTexture(node->texture);
            free(node->key);
            free(node);

            node = next;
        }
    }
}

SDL_Rect GetScaledTextureSize(SDL_Texture * texture, int draw_scale)
{
    float scale_x;
    float scale_y;
    SDL_RenderGetScale(renderer, &scale_x, &scale_y);
    printf("scale: %f, %f\n", scale_x, scale_y);

    int width;
    int height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect result = {
        0,
        0,
        width * scale_x * draw_scale,
        height * scale_y * draw_scale
    };

    return result;
}
