cmake_minimum_required(VERSION 3.9)

project(smite)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

set(SMITE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/details/fake_ptr.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/details/storage.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/details/compressed_pair.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/range.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/transform_iterator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/enumerate_iterator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/filter_iterator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/zip_iterator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/smite/smite.hpp
        )

add_library(smite INTERFACE)

target_sources(smite INTERFACE ${SMITE_SOURCES})

target_include_directories(smite INTERFACE include)

find_package(GTest REQUIRED)

add_executable(smite-tests
        tests/smite-tests.cpp
        )

target_link_libraries(smite-tests smite GTest::GTest GTest::Main)
