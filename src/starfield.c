#include "starfield.h"
#include "globals.h"

void starfield_init(Starfield* self, SDL_Renderer* renderer, u32 width, u32 height, u32 starCount) {
    self->width = width; self->height = height; self->starCount = starCount;
    self->texture = NULL;
    starfield_regenerate(self, renderer);
}

void starfield_regenerate(Starfield* self, SDL_Renderer* renderer) {
    if (self->texture) {
        SDL_DestroyTexture(self->texture);
    }

    Vec2 seedPositions[STARFIELD_SEEDED_LOCATIONS];
    for (u32 i = 0; i < STARFIELD_SEEDED_LOCATIONS; ++i) {
        seedPositions[i] = vec2_rand_range(0.f, 0.f, (f32)self->width, (f32)self->height);
    }

    SDL_Surface* surface = SDL_CreateRGBSurface(0, self->width, self->height, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

    for (u32 i = 0; i < self->starCount; ++i) {
        Vec2 seed = seedPositions[rand() % STARFIELD_SEEDED_LOCATIONS];
        f32 distance = randf_range(0.f, 200.f);
        f32 angle = randf_range(0.f, 360.f);
        Vec2 position;
        vec2_set_angle(&position, angle, distance);
        vec2_add(&seed, &position, &position);

        u32 x = (u32)position.x;
        u32 y = (u32)position.y;
        Color color = color_make_rgbaf(randf(1.f), randf(1.f), randf(1.f), 1.f);

        SDL_Rect pixel = {
            x, y, 1, 1
        };

        u32 packedColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

        SDL_FillRect(surface, &pixel, packedColor);
    }

    self->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void starfield_cleanup(Starfield* self) {
    SDL_DestroyTexture(self->texture);
}

void starfield_render(Starfield* self, SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, self->texture, NULL, NULL);
}