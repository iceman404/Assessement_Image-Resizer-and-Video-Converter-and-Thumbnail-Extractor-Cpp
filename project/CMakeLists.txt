# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Name of the project
project(VideoConverterAndResizer)

# List of source files for the project
set(SOURCES
    src/JpegResizer.cpp
    src/PngResizer.cpp
    src/ImageResizerFactory.cpp
    src/WebMConverter.cpp
    src/ThumbnailExtractor.cpp
    src/VideoConverterFactory.cpp
    src/main.cpp)

# Include the header files directory
include_directories(include)

# Find FFmpeg
find_package(FFmpeg REQUIRED)

# Check if FFmpeg libraries were found
if(NOT FFmpeg_FOUND)
    message(FATAL_ERROR "FFmpeg not found. Please install the FFmpeg development libraries.")
endif()

# Create the main executable from the specified source files
add_executable(VideoConverterAndResizer ${SOURCES})

# Link the FFmpeg libraries to the main executable
target_link_libraries(VideoConverterAndResizer
    ${FFmpeg_LIBRARIES}
)

# Option to enable testing
option(ENABLE_TESTING "Enable testing" OFF)

# Conditionally enable testing
if(ENABLE_TESTING)

    # Enable testing for the project
    enable_testing()

    # Find the GTest module for unit testing
    find_package(GTest REQUIRED)

    # Include the directories for GTest
    include_directories(${GTEST_INCLUDE_DIRS})

    # Create the test executable from the specified test files
    add_executable(runTests
        tests/TestJpegResizer.cpp
        tests/TestPngResizer.cpp
        tests/TestImageResizerFactory.cpp
        tests/TestVideoConverter.cpp)

    # Link the GTest libraries and the FFmpeg libraries to the test executable
    target_link_libraries(runTests GTest::GTest GTest::Main ${FFmpeg_LIBRARIES})

    # Register the test executable with CTest
    add_test(AllTests runTests)

endif()

message(STATUS "CMake configuration done. You can now build the project.")
