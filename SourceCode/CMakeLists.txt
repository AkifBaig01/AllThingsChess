cmake_minimum_required(VERSION 3.10)
project(MyChessEngine VERSION 0.1.0 LANGUAGES CXX)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(MyChessEngine
    bitboard.cpp
    uci.cpp
    utils.cpp
    movegen.cpp
    moves.cpp
    evaluate.cpp
    search.cpp
    main.cpp
)

target_include_directories(MyChessEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET MyChessEngine PROPERTY CXX_STANDARD 17)

# Optimisation flags for GCC / Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MyChessEngine PRIVATE
        -O3
        -march=native
        -DNDEBUG
    )
endif()
