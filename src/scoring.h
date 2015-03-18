#ifndef ASTEROIDS_SCORING_H
#define ASTEROIDS_SCORING_H

#include "memory.h"
#include "types.h"
#include "debug.h"
#include "log.h"

#define SCORE_BOARD_MAX_NAME_LENGTH 128

typedef struct score_t {
    char name[SCORE_BOARD_MAX_NAME_LENGTH];
    i32 score;
} Score;

typedef struct score_board_t {
    Score* scores;
    u32 capacity;
} ScoreBoard;

void score_board_init(ScoreBoard* self, FILE* scoreFile, u32 max);
void score_board_dump(ScoreBoard* self, FILE* file);
Score score_board_next_high(ScoreBoard* self, i32 score);
i32 score_board_insert(ScoreBoard* self, char* name, i32 score);

#endif