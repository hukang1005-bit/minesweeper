#include "minesweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

static void draw(const Board *b) {
    printf("\n    ");
    for (int x = 0; x < b->width; ++x)
        printf("%2d ", x + 1);
    putchar('\n');
    for (int y = 0; y < b->height; ++y) {
        printf("%3d ", y + 1);
        for (int x = 0; x < b->width; ++x) {
            char cell = '#';
            if (b->lost && b->cells[y][x] == 9)
                cell = '*';
            else if (b->flagged[y][x])
                cell = 'F';
            else if (b->revealed[y][x])
                cell = b->cells[y][x] ? (char)('0' + b->cells[y][x]) : '.';
            printf(" %c ", cell);
        }
        putchar('\n');
    }
    printf("Safe cells remaining: %d\n", b->remaining);
}

int main(int argc, char **argv) {
    int width = 9, height = 9, mines = 10;
    if (argc != 1 && argc != 4) {
        fprintf(stderr, "Usage: %s [width height mines]\n", argv[0]);
        return 1;
    }
    if (argc == 4) {
        int *values[] = {&width, &height, &mines};
        for (int i = 1; i < 4; ++i) {
            char *end;
            long value = strtol(argv[i], &end, 10);
            if (!*argv[i] || *end || value < 0 || value > 1800) {
                fputs("Invalid board parameters.\n", stderr);
                return 1;
            }
            *values[i - 1] = (int)value;
        }
    }
    Board board;
    if (!ms_init(&board, width, height, mines, (unsigned)time(NULL))) {
        fputs("Width: 1-60; height: 1-30; mines: 0 to width*height-1.\n", stderr);
        return 1;
    }
    puts("Minesweeper | r column row: reveal | f column row: flag | q: quit");
    char line[128], action, extra;
    int x, y;
    while (!board.lost && !ms_won(&board)) {
        draw(&board);
        printf("> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin) || line[0] == 'q')
            return 0;
        if (sscanf(line, " %c %d %d %c", &action, &x, &y, &extra) != 3) {
            puts("Enter r/f followed by column and row.");
            continue;
        }
        if (x < 1 || x > board.width || y < 1 || y > board.height) {
            puts("Coordinates are outside the board.");
            continue;
        }
        bool changed = action == 'r'   ? ms_reveal(&board, x - 1, y - 1)
                       : action == 'f' ? ms_flag(&board, x - 1, y - 1)
                                       : false;
        if (!changed)
            puts("Invalid action or cell is unavailable.");
    }
    draw(&board);
    puts(board.lost ? "Mine hit. Game over." : "All safe cells revealed. You win!");
    return 0;
}
