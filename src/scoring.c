#include "scoring.h"

void score_board_init(ScoreBoard* self, FILE* scoreFile, u32 max) {
    self->capacity = max;
    self->scores = CALLOC(self->capacity, Score);
    for (u32 i = 0; i < self->capacity; ++i) {
        self->scores[i].name = NULL;
        self->scores[i].score = 0;
    }

    // TODO: read score file
}

void score_board_dump(ScoreBoard* self, FILE* file) {

}

Score score_board_next_high(ScoreBoard* self, i32 score) {
    for (int i = self->capacity; i > 0; --i) {
        Score s = self->scores[i - 1];
        if (s.score >= score) {
            return s;
        }
    }

    Score currentScore = {
        NULL,
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

    if (insertionIndex >= self->capacity) {
        return -1;
    }

    for (i32 i = insertionIndex; i < self->capacity - 1; ++i) {
        self->scores[i + 1] = self->scores[i];
    }

    self->scores[insertionIndex].name = name;
    self->scores[insertionIndex].score = score;
}