cmake_minimum_required(VERSION 3.24)
project(AlgebraicSystem)

set(CMAKE_CXX_STANDARD 23)

include_directories(./include)

set(HEADERS
        include/AlgebraicSystem.hpp
        )

set(SOURCES
        main.cpp
)

set(TEST-SOURCES
        test/main.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

add_executable(Test ${HEADERS} ${TEST-SOURCES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(Test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)