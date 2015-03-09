#ifndef ASTEROIDS_STARFIELD_H
#define ASTEROIDS_STARFIELD_H

#include "algebra.h"
#include "color.h"
#include "primitives.h"

#define STARFIELD_SEEDED_LOCATIONS 25

typedef struct starfield_star_t {
    Vec2 position;
    Color color;
} StarfieldStar;

typedef struct starfield_t {
    SDL_Texture* texture;
    u32 width, height, starCount;
} Starfield;

void starfield_init(Starfield* self, SDL_Renderer* renderer, u32 width, u32 height, u32 starCount);
void starfield_regenerate(Starfield* self, SDL_Renderer* renderer);
void starfield_cleanup(Starfield* self);
void starfield_render(Starfield* self, SDL_Renderer* renderer);

#endif