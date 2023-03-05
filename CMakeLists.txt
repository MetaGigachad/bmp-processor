project(bmp-processor)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)

set(SOURCE_FILES
        core/app.cpp
        core/bitmap.cpp
        core/parser.cpp
        filters/filter_pipeline.cpp
        filters/filters.cpp
        exceptions/app_error.cpp)
add_executable(image_processor main.cpp ${SOURCE_FILES})
target_include_directories(image_processor PUBLIC core filters exceptions)

set(TEST_FILES
        tests/test.cpp)
add_catch(test_image_processor ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(test_image_processor PUBLIC core filters exceptions)
