#include "minesweeper.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    Board b;
    assert(!ms_init(&b, 0, 9, 1, 1));
    assert(!ms_init(&b, 2, 2, 4, 1));
    assert(ms_init(&b, 60, 30, 0, 1));
    assert(ms_reveal(&b, 0, 0) && ms_won(&b));
    for (int width = 1; width <= 17; ++width) {
        for (int height = 1; height <= 13; ++height) {
            int mines = width * height / 3;
            assert(ms_init(&b, width, height, mines, 42));
            int found = 0;
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x) {
                    if (b.cells[y][x] == 9) {
                        ++found;
                        continue;
                    }
                    int neighbors = 0;
                    for (int dy = -1; dy <= 1; ++dy)
                        for (int dx = -1; dx <= 1; ++dx)
                            if (y + dy >= 0 && y + dy < height && x + dx >= 0 && x + dx < width)
                                neighbors += b.cells[y + dy][x + dx] == 9;
                    assert(b.cells[y][x] == neighbors);
                }
            assert(found == mines);
            assert(!ms_reveal(&b, -1, 0));
            assert(!ms_flag(&b, width, height));
            assert(ms_flag(&b, 0, 0));
            assert(!ms_reveal(&b, 0, 0));
            assert(ms_flag(&b, 0, 0));
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x)
                    if (b.cells[y][x] != 9)
                        ms_reveal(&b, x, y);
            assert(ms_won(&b));
        }
    }
    assert(ms_init(&b, 4, 3, 11, 1));
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 4; ++x)
            if (b.cells[y][x] == 9) {
                ms_reveal(&b, x, y);
                assert(b.lost);
                return 0;
            }
    return 1;
}
