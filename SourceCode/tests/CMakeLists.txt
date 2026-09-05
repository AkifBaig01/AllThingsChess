cmake_minimum_required(VERSION 3.14)

set(this TestEngine)

set(Sources
    exampleTest.cpp
)

add_executable(${this}
    ${Sources}
)

target_link_libraries(${this}
    PRIVATE
        ${lib}
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(${this})