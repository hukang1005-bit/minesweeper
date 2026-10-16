#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <stdbool.h>
#define MS_MAX_WIDTH 60
#define MS_MAX_HEIGHT 30
typedef struct {
    int width, height, mines, remaining;
    bool lost;
    unsigned char cells[MS_MAX_HEIGHT][MS_MAX_WIDTH];
    bool revealed[MS_MAX_HEIGHT][MS_MAX_WIDTH];
    bool flagged[MS_MAX_HEIGHT][MS_MAX_WIDTH];
} Board;
bool ms_init(Board *board, int width, int height, int mines, unsigned seed);
bool ms_reveal(Board *board, int x, int y);
bool ms_flag(Board *board, int x, int y);
bool ms_won(const Board *board);
#endif
