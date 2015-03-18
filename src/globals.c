#include "globals.h"
#include "gamescene.h"
#include "game.h"
#include <time.h>
#include "scoring.h"

Globals globals;

void globals_init() {
    globals.randomSeed = 0;

    globals.screen.width = 0;
    globals.screen.height = 0;
    globals.screen.fullscreen = false;

    globals.world.width = 0;
    globals.world.height = 0;

    game_time_initialize(&globals.time);

    globals.window = NULL;
    globals.renderer = NULL;
    
    globals.game = NULL;
}

void globals_config(const char* configName) {
    Config* config = config_get(configName);

    globals.randomSeed = (u32)config_try_get_int(config, INI_GLOBAL, "random_seed", 0);

    if (globals.randomSeed == 0) {
        globals.randomSeed = (u32)time(NULL);
    }

    srand(globals.randomSeed);

    globals.screen.width = (u32)config_try_get_int(config, "screen", "width", 1280);
    globals.screen.height = (u32)config_try_get_int(config, "screen", "height", 720);
    globals.screen.fullscreen = config_try_get_bool(config, "screen", "fullscreen", false);

    globals.world.width = (u32)config_try_get_int(config, "world", "width", 1280);
    globals.world.height = (u32)config_try_get_int(config, "world", "height", 720);

    globals.scrollSpeed = config_try_get_float(config, "game", "scroll_speed", 0.f);
    globals.levelPosition = 0.f;

    Color clearColor = CONFIG_TRY_GET(Color)(config, "game", "clear_color", color_make_rgb(0, 0, 0));
    globals.clearColor = *((SDL_Color*)&clearColor);
}

void textures_init(const char* rootDir) { texture_manager_init(&globals.textures, rootDir); }
void textures_terminate() { texture_manager_free(&globals.textures); }
bool textures_load(const char* filename) { return texture_manager_load(&globals.textures, globals.renderer, filename); }
bool textures_unload(const char* filename) { return texture_manager_unload(&globals.textures, filename); }
bool textures_unload_all() { return texture_manager_unload_all(&globals.textures); }
SDL_Texture* textures_get(const char* name) { return texture_manager_get(&globals.textures, name); }

GameScene* active_scene() {
    return globals.game->activeScene;
}

Timer* timer_add(Entity target, Message message, f32 delay) {
    return timer_manager_add(active_scene()->timerManager, message, target, delay);
}

Timer* timer_add_interval(Entity target, Message message, f32 delay, f32 interval) {
    return timer_manager_add_interval(active_scene()->timerManager, message, target, delay, interval);
}

void timer_remove(Timer* timer) {
    timer_manager_remove(active_scene()->timerManager, timer);
}

void load_scores(ScoreBoard* scores, u32 max) {
    FILE* scoreFile = fopen("scores.txt", "r");
    if (!scoreFile) {
        scoreFile = fopen("assets/data/default_scores.txt", "r");
        ASSERT(scoreFile, "Failed to find score file or default score file backup.");
    }
    score_board_init(scores, scoreFile, max);
    fclose(scoreFile);
}

void save_scores(ScoreBoard* scores) {
    FILE* scoreFile = fopen("scores.txt", "w");
    score_board_dump(scores, scoreFile);
    fclose(scoreFile);
}