#include "application.h"

int app_run(int argc, char* argv[]) {
    App* self = _app_new();

    IF_DEBUG(bool appInit = )_app_initialize(self);
    ASSERT(appInit, "Application failed to initialize");

    game_init(&self->game);
    game_start(&self->game);

    while (!self->shouldQuit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            _app_handle_event(self, &event);
        }

        _app_update(self);
        _app_render(self);
    }

    game_quit(&self->game);
    _app_terminate(self);

    return 0;
}

void _app_handle_event(App* self, SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            self->shouldQuit = true;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            input_handle_event(&event->key);
            break;
    }
}

void _app_update(App* self) {
    game_time_update(&globals.time);

    if (input_key_down(SDL_SCANCODE_ESCAPE)) {
        self->shouldQuit = true;
    }

    game_update(&self->game);

    input_update();
}

void _app_render(App* self) {
    SDL_SetRenderDrawColor(globals.renderer, 31, 31, 31, 255);
    SDL_RenderClear(globals.renderer);

    game_render(&self->game);

    SDL_RenderPresent(globals.renderer);
}

void app_quit(App* self) {
    ASSERT(self, "NULL application structure passed to app_quit");

    self->shouldQuit = true;
}

bool _app_initialize(App* self) {
    globals_init();

    globals.randomSeed = (u32)time(NULL);
    srand(globals.randomSeed);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("SDL", SDL_GetError());
        return false;
    }

    globals.world.width = 1280;
    globals.world.height = 720;

    globals.screen.width = globals.world.width / 1;
    globals.screen.height = globals.world.height / 1;

    globals.window = window_create("Runner",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        globals.screen.width,
        globals.screen.height,
        SDL_WINDOW_SHOWN);

    if (!globals.window) {
        return false;
    }

    globals.renderer = SDL_CreateRenderer(globals.window, -1, SDL_RENDERER_ACCELERATED);
    if (!globals.renderer) {
        log_error("SDL", SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(globals.renderer, globals.world.width, globals.world.height);

    self->shouldQuit = false;

    input_initialize();

    globals.time.on_second = _app_print_fps;

    return true;
}

void _app_terminate(App* self) {
    SDL_DestroyWindow(globals.window);
    SDL_DestroyRenderer(globals.renderer);

    SDL_Quit();

    _app_free(self);
}

App* _app_new() {
    App* self = (App*)calloc(1, sizeof(App));

    self->shouldQuit = false;

    return self;
}

void _app_free(App* self) {
    free(self);
}

void _app_print_fps() {
    printf("FPS: %d\n", globals.time.fps);
}