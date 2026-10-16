#include "minesweeper.h"
#include <stdlib.h>
#include <string.h>

static bool inside(const Board *b, int x, int y) {
    return b && x >= 0 && y >= 0 && x < b->width && y < b->height;
}

bool ms_init(Board *b, int width, int height, int mines, unsigned seed) {
    if (!b || width < 1 || width > MS_MAX_WIDTH || height < 1 || height > MS_MAX_HEIGHT ||
        mines < 0 || mines >= width * height)
        return false;
    memset(b, 0, sizeof(*b));
    b->width = width;
    b->height = height;
    b->mines = mines;
    b->remaining = width * height - mines;
    srand(seed);
    /* Partial Fisher-Yates avoids repeated placement attempts on dense boards. */
    int positions[MS_MAX_WIDTH * MS_MAX_HEIGHT];
    int count = width * height;
    for (int i = 0; i < count; ++i)
        positions[i] = i;
    for (int i = 0; i < mines; ++i) {
        int j = i + rand() % (count - i);
        int temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
        int pos = positions[i];
        b->cells[pos / width][pos % width] = 9;
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (b->cells[y][x] == 9)
                continue;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    if (inside(b, x + dx, y + dy) && b->cells[y + dy][x + dx] == 9)
                        ++b->cells[y][x];
        }
    }
    return true;
}

bool ms_flag(Board *b, int x, int y) {
    if (!inside(b, x, y) || b->lost || ms_won(b) || b->revealed[y][x])
        return false;
    b->flagged[y][x] = !b->flagged[y][x];
    return true;
}

bool ms_reveal(Board *b, int x, int y) {
    if (!inside(b, x, y) || b->lost || ms_won(b) || b->flagged[y][x] || b->revealed[y][x])
        return false;
    if (b->cells[y][x] == 9) {
        b->lost = true;
        return true;
    }
    int queue[MS_MAX_WIDTH * MS_MAX_HEIGHT];
    int head = 0, tail = 0;
    b->revealed[y][x] = true;
    --b->remaining;
    queue[tail++] = y * b->width + x;
    /* Mark cells when enqueuing, so each cell uses at most one queue slot. */
    while (head < tail) {
        int pos = queue[head++];
        int cx = pos % b->width, cy = pos / b->width;
        if (b->cells[cy][cx] != 0)
            continue;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = cx + dx, ny = cy + dy;
                if (!inside(b, nx, ny) || b->revealed[ny][nx] || b->flagged[ny][nx] ||
                    b->cells[ny][nx] == 9)
                    continue;
                b->revealed[ny][nx] = true;
                --b->remaining;
                queue[tail++] = ny * b->width + nx;
            }
        }
    }
    return true;
}

bool ms_won(const Board *b) { return b && b->width > 0 && !b->lost && b->remaining == 0; }
