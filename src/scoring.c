#include "scoring.h"
#include <string.h>
#include "snprintf.h"

void score_board_init(ScoreBoard* self, FILE* scoreFile, u32 max) {
    self->capacity = max;
    self->scores = CALLOC(self->capacity, Score);
    for (u32 i = 0; i < self->capacity; ++i) {
        self->scores[i].score = 0;
    }

    u32 index = 0;
    char currentName[SCORE_BOARD_MAX_NAME_LENGTH];
    i32 currentScore = 0;

    while (fscanf(scoreFile, "%s %d\n", currentName, &currentScore)) {
        if (index >= self->capacity) {
            break;
        }

        snprintf(self->scores[index].name, SCORE_BOARD_MAX_NAME_LENGTH, "%s", currentName);
        self->scores[index].score = currentScore;

        ++index;
    }

    // Fill in unused spots
    for (u32 i = index; i < self->capacity; ++i) {
        snprintf(self->scores[index].name, SCORE_BOARD_MAX_NAME_LENGTH, "AAA");
        self->scores[i].score = 0;
    }
}

void score_board_dump(ScoreBoard* self, FILE* file) {
    for (u32 i = 0; i < self->capacity; ++i) {
        fprintf(file, "%s %d\n", self->scores[i].name, self->scores[i].score);
    }
}

Score score_board_next_high(ScoreBoard* self, i32 score) {
    for (int i = self->capacity; i > 0; --i) {
        Score s = self->scores[i - 1];
        if (s.score >= score) {
            return s;
        }
    }

    Score currentScore = {
        { 0 },
        score
    };

    return currentScore;
}

i32 score_board_insert(ScoreBoard* self, char* name, i32 score) {
    i32 insertionIndex = -1;
    for (int i = self->capacity; i > 0; --i) {
        Score s = self->scores[i - 1];
        if (s.score >= score) {
            insertionIndex = i;
        }
    }

    if (insertionIndex >= (i32)self->capacity) {
        return -1;
    }

    for (i32 i = insertionIndex; i < (i32)self->capacity - 1; ++i) {
        self->scores[i + 1] = self->scores[i];
    }

    snprintf(self->scores[insertionIndex].name, SCORE_BOARD_MAX_NAME_LENGTH, "%s", name);
    self->scores[insertionIndex].score = score;

    return insertionIndex;
}