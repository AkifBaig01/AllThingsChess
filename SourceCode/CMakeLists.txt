cmake_minimum_required(VERSION 3.14)

set(lib ChessLib)
set(engine MyChessEngine)

project(${engine} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

set(Headers
    bitboard.h
    uci.h
    movegen.h
    moves.h
    search.h
    evaluate.h
)

set(Sources
    bitboard.cpp
    uci.cpp
    utils.cpp
    movegen.cpp
    moves.cpp
    evaluate.cpp
    search.cpp
)

add_library(${lib} STATIC 
    ${Headers} ${Sources}
)

target_include_directories(${lib} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${engine}
    main.cpp
)

target_link_libraries(${engine} PRIVATE
    ${lib}
)

add_subdirectory(tests)
