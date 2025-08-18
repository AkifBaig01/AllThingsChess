cmake_minimum_required(VERSION 3.10)
project(MyChessEngine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Add all your source files from the SourceCode directory
add_executable(MyChessEngine
    bitboard.cpp
    uci.cpp
    utils.cpp
    movegen.cpp
    main.cpp
)

# If you have headers, and want to include them globally
target_include_directories(MyChessEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET MyChessEngine PROPERTY CXX_STANDARD 17)