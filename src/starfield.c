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
        f32 distance = randf_range(0.f, 250.f);
        f32 angle = randf_range(0.f, 360.f);
        Vec2 position;
        vec2_set_angle(&position, angle, distance);
        vec2_add(&seed, &position, &position);

        const f32 cMinSize = 1.f;
        const f32 cMaxSize = 5.f;
        f32 size = randf_range(cMinSize, cMaxSize);

        int x = (int)position.x;
        int y = (int)position.y;
        
        Color yellow = color_make_rgb(255, 255, 96);
        Color blue = color_make_rgb(107, 136, 255);
        Color color;
        color_lerpf(&yellow, &blue, randf(1.f), &color);

        if (randf(100.f) < 2.f) {
            color = color_make_rgb(255, 0, 0);
        }

        color.a = (u8)(((cMaxSize - size) - cMinSize) / (cMaxSize - cMinSize) * 255);

        int isize = (int)size;
        int halfSize = isize / 2;
        
        SDL_Rect bgRect = {
            x - halfSize, y - halfSize, isize, isize
            //x, y, 1, 1
        };
        u32 packedColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 31);
        SDL_FillRect(surface, &bgRect, packedColor);
        
        SDL_Rect pixel = {
            //x - halfSize, y - halfSize, isize, isize
            x, y, 1, 1
        };
        packedColor = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
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