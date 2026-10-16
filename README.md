# Minesweeper

A terminal Minesweeper game in C11. The board engine is independent of input and rendering, with bounded storage and iterative flood fill.

## Build and run

Requires a C11 compiler and CMake 3.16 or later. From the repository root:

```sh
cmake -S . -B build
cmake --build build --config Release
./build/minesweeper
```

For a Visual Studio generator on Windows, run `build\Release\minesweeper.exe`. Visual Studio can also open this folder as a CMake project.

## Controls

Default board: 9 × 9, 10 mines. Use one-based coordinates:

```text
r 3 5     reveal column 3, row 5
f 3 5     toggle a flag
q         quit
```

Custom board: `./build/minesweeper 16 12 30`. Width is limited to 60, height to 30, and the mine count must leave at least one safe cell. Reveal all safe cells to win. The first move may hit a mine. Flags never change the underlying board.

## Tests

```sh
ctest --test-dir build -C Release --output-on-failure
```

Core regression tests run without a terminal. They exercise rectangular boards, dense mine placement, boundary clicks, flood fill, flag toggling, and win/loss rules. The GitHub Actions workflow builds and tests on Windows, macOS, and Linux.

## Layout

- `include/`: public game state and engine API
- `src/minesweeper.c`: game rules
- `src/main.c`: terminal interface
- `tests/`: core regression checks

## Project history

Refactored from the supplied `MineSweeper-main.zip` project. The original Windows console implementation was reorganized around a portable game engine. The current interface uses typed commands in place of Windows-only mouse events. Original archives and build outputs are retained only in the local backup.
