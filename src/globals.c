#include "globals.h"
#include <time.h>

Globals globals;

void globals_init() {
    globals.randomSeed = 0;

    globals.screen.width = 0;
    globals.screen.height = 0;

    globals.world.width = 0;
    globals.world.height = 0;

    game_time_initialize(&globals.time);

    globals.window = NULL;
    globals.renderer = NULL;
}

void globals_config(const char* configName) {
 //   Ini* config = config_get(configName);

 //   globals.randomSeed = (u32)ini_try_get_int(config, INI_GLOBAL, "random_seed", 0);

 //   if (globals.randomSeed == 0) {
 //       globals.randomSeed = (u32)time(NULL);
 //       srand(globals.randomSeed);
 //   }

 //   globals.screen.width = (u32)ini_try_get_int(config, "screen", "width", 1280);
 //   globals.screen.height = (u32)ini_try_get_int(config, "screen", "height", 720);

 //   globals.world.width = (u32)ini_try_get_int(config, "world", "width", 1280);
 //   globals.world.height = (u32)ini_try_get_int(config, "world", "height", 720);

 //   globals.scrollSpeed = ini_try_get_float(config, "game", "scroll_speed", 0.f);

	//tween_manager_init(&globals.tweens, 512);
}

void textures_init(const char* rootDir) { texture_manager_init(&globals.textures, rootDir); }
void textures_free() { texture_manager_free(&globals.textures); }
bool textures_load(const char* filename) { return texture_manager_load(&globals.textures, globals.renderer, filename); }
bool textures_unload(const char* filename) { return texture_manager_unload(&globals.textures, filename); }
bool textures_unload_all() { return texture_manager_unload_all(&globals.textures); }
SDL_Texture* textures_get(const char* name) { return texture_manager_get(&globals.textures, name); }