# AllThingsChess

AllThingsChess is a chess engine written from scratch in C++17 as a personal systems-programming project. It uses bitboards for position representation, precomputed attack tables for move generation, a reversible make/unmake system, and negamax search with alpha-beta pruning.

The engine is a working first prototype and is currently being optimised. It has been developed and tested on both Windows and Linux, and its UCI interface has been configured successfully with Arena Chess GUI.

## Current features

- Bitboard-based board representation with a 64-square piece map
- FEN parsing for arbitrary positions
- Precomputed pawn, knight and king attack tables
- Magic-bitboard attack generation for bishops and rooks
- Pseudo-legal and fully legal move generation
- Castling, en passant, captures, double pawn pushes and all promotion types
- Reversible make/unmake operations with restoration of game state
- Checkmate, stalemate, 50-move-rule and insufficient-material detection
- Material and piece-square-table evaluation
- Negamax search with alpha-beta pruning
- Core UCI commands for setting positions, searching to a fixed depth and returning a best move
- Perft testing against published reference positions
- Debug, release and profiling configurations through CMake Presets

## Project structure

The engine source is contained in `SourceCode/`.

| File | Responsibility |
| --- | --- |
| `bitboard.cpp` / `bitboard.h` | Builds and stores the complete bitboard position representation |
| `movegen.cpp` / `movegen.h` | Generates leaper attacks and magic-bitboard sliding-piece attack tables |
| `moves.cpp` / `moves.h` | Generates moves, filters for legality, and applies or reverses moves |
| `evaluate.cpp` / `evaluate.h` | Evaluates positions and detects terminal game states |
| `search.cpp` / `search.h` | Implements negamax search with alpha-beta pruning |
| `uci.cpp` / `uci.h` | Parses FEN strings and constructs the initial board state |
| `uci_real.cpp` | Handles the engine's UCI command loop |
| `utils.cpp` / `utils.h` | Provides board-coordinate, bit-manipulation and debugging utilities |
| `main.cpp` | Contains the current perft and development test harness |

## Requirements

- CMake 3.10 or later
- A compiler with C++17 support
- Windows: MinGW/GCC
- Linux: GCC and Ninja

The supplied presets are intended to be selected through the Microsoft CMake Tools extension in Visual Studio Code.

## Building

Clone the repository and enter the source directory:

```bash
git clone https://github.com/AkifBaig01/AllThingsChess.git
cd AllThingsChess/SourceCode
```

### Visual Studio Code

1. Open `SourceCode/` in Visual Studio Code.
2. Run **CMake: Select Configure Preset** from the Command Palette.
3. Select the required preset.
4. Run **CMake: Build**.

Available Linux presets:

- `linux-debug` — normal development and debugging
- `linux-release-o3` — optimised release build
- `linux-profile` — optimised build with debug symbols for performance profiling

Available Windows/MinGW presets:

- `default` — debug build
- `release-o3` — optimised release build
- `profile` — build configured for profiling with `gprof`

Build output is written to the corresponding `build-*` directory inside `SourceCode/`.

## Running the engine

The project currently uses `main.cpp` as a development harness. Perft testing is enabled by default on the current optimisation branch. The UCI loop can be enabled in `main.cpp` when testing the engine through a terminal or Arena.

Once UCI mode is enabled, a basic terminal session is:

```text
uci
isready
position startpos
go depth 6
quit
```

The interface also accepts move sequences and FEN positions, including:

```text
position startpos moves e2e4 e7e5 g1f3
go depth 6
```

```text
position fen <FEN string>
go depth 6
```

The engine has also been configured as a UCI engine in Arena and can search and return moves during play.

## Testing

Move generation is tested using perft, which counts every legal leaf node to a specified depth and allows the result to be compared with established reference counts.

Starting-position results include:

| Depth | Reference nodes | Engine result |
| ---: | ---: | ---: |
| 6 | 119,060,324 | 119,060,324 |
| 7 | 3,195,901,860 | 3,195,901,860 |

Additional reference positions from the Chess Programming Wiki and chess-programming community have been used to exercise castling, en passant, promotion, checks, captures and other move-generation edge cases.

## Performance

The current optimisation branch processes approximately **26 million nodes per second** in starting-position perft tests at depths 6 and 7. This was measured using the Linux profiling configuration on an AMD Ryzen 7 4700U.

Performance depends on the compiler, build configuration, hardware and test position, so this figure is intended as a development benchmark rather than a cross-engine comparison.

Profiling identified move generation, make/unmake operations and state updates as the main performance costs. The first completed change replaces dynamically allocated move lists with a fixed-capacity `MoveList`:

```cpp
struct MoveList {
    Move moves[256];
    int count = 0;
};
```

This removes repeated allocation from the main move-generation path. The undo history still uses a dynamic container and is part of the current optimisation work.

## Current limitations

- Perft and UCI execution modes are selected manually in `main.cpp`.
- Search is single-threaded and operates to a fixed depth.
- Evaluation is currently limited to material values and piece-square tables.
- The search does not yet include advanced features such as a transposition table or quiescence search.
- Some supporting code still uses dynamic containers in performance-sensitive paths.

## Short-term roadmap

1. Replace the dynamic undo-history container with fixed or preallocated storage.
2. Pack each `Move` into a more compact representation.
3. Reduce the amount of work performed by make/unmake operations.
4. Re-run consistent perft and profiling benchmarks after each change.
5. Merge the completed optimisation work into the main branch.

## References and acknowledgements

- The magic-bitboard constants were generated as part of this project using the magic-bitboard technique described by the [Chess Programming Wiki](https://www.chessprogramming.org/Magic_Bitboards).
- The current piece-square tables are adapted from the [Chess Programming Wiki's simplified evaluation material](https://www.chessprogramming.org/Simplified_Evaluation_Function).
- Perft results are checked against published [perft reference results](https://www.chessprogramming.org/Perft_Results).

## Status

This project began as my first large independent C++ program. The current version is a functional prototype that can generate legal chess moves, validate positions through perft, search positions and communicate through core UCI commands. Development is now focused on profiling and improving the engine's internal data structures and state-update performance.
