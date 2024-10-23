#@Author: John Subba
#Date Created: 10-23-2024

#!/bin/bash

# Create the project directory structure
mkdir -p project/{include,src,build,bin,tests}

# Header files (include directory)
touch project/include/IImageResizer.h
touch project/include/JpegResizer.h
touch project/include/PngResizer.h
touch project/include/ImageResizerFactory.h
touch project/include/IVideoConverter.h
touch project/include/WebMConverter.h
touch project/include/ThumbnailExtractor.h
touch project/include/VideoConverterFactory.h

# Source files (src directory)
touch project/src/JpegResizer.cpp
touch project/src/PngResizer.cpp
touch project/src/ImageResizerFactory.cpp
touch project/src/WebMConverter.cpp
touch project/src/ThumbnailExtractor.cpp
touch project/src/VideoConverterFactory.cpp
touch project/src/main.cpp

# Test files (tests directory)
touch project/tests/TestJpegResizer.cpp
touch project/tests/TestPngResizer.cpp
touch project/tests/TestImageResizerFactory.cpp
touch project/tests/TestVideoConverter.cpp

# CMakeLists.txt
echo "cmake_minimum_required(VERSION 3.10)" > project/CMakeLists.txt
echo "project(VideoConverterAndResizer)" >> project/CMakeLists.txt

# Add the include and source files to CMakeLists.txt
echo "set(SOURCES" >> project/CMakeLists.txt
echo "    src/JpegResizer.cpp" >> project/CMakeLists.txt
echo "    src/PngResizer.cpp" >> project/CMakeLists.txt
echo "    src/ImageResizerFactory.cpp" >> project/CMakeLists.txt
echo "    src/WebMConverter.cpp" >> project/CMakeLists.txt
echo "    src/ThumbnailExtractor.cpp" >> project/CMakeLists.txt
echo "    src/VideoConverterFactory.cpp" >> project/CMakeLists.txt
echo "    src/main.cpp)" >> project/CMakeLists.txt

echo "include_directories(include)" >> project/CMakeLists.txt

# Link necessary libraries for libav
echo "find_package(PkgConfig REQUIRED)" >> project/CMakeLists.txt
echo "pkg_check_modules(LIBAV REQUIRED libavcodec libavformat libswscale libavutil)" >> project/CMakeLists.txt
echo "include_directories(\${LIBAV_INCLUDE_DIRS})" >> project/CMakeLists.txt
echo "add_executable(VideoConverterAndResizer \${SOURCES})" >> project/CMakeLists.txt
echo "target_link_libraries(VideoConverterAndResizer \${LIBAV_LIBRARIES})" >> project/CMakeLists.txt

# GTest integration for unit tests
echo "enable_testing()" >> project/CMakeLists.txt
echo "find_package(GTest REQUIRED)" >> project/CMakeLists.txt
echo "include_directories(\${GTEST_INCLUDE_DIRS})" >> project/CMakeLists.txt

# Add test files to the build
echo "add_executable(runTests" >> project/CMakeLists.txt
echo "    tests/TestJpegResizer.cpp" >> project/CMakeLists.txt
echo "    tests/TestPngResizer.cpp" >> project/CMakeLists.txt
echo "    tests/TestImageResizerFactory.cpp" >> project/CMakeLists.txt
echo "    tests/TestVideoConverter.cpp)" >> project/CMakeLists.txt

echo "target_link_libraries(runTests GTest::GTest GTest::Main \${LIBAV_LIBRARIES})" >> project/CMakeLists.txt
echo "add_test(AllTests runTests)" >> project/CMakeLists.txt

# Build instructions
echo "To build the project:" > project/README.md
echo "1. Run 'cmake -S . -B build' in the project root directory." >> project/README.md
echo "2. Run 'cmake --build build' to compile the project." >> project/README.md
echo "3. Run 'ctest' to execute the tests." >> project/README.md

# Add dependencies and requirements
echo "Dependencies and Requirements:" >> project/README.md
echo "- libavcodec" >> project/README.md
echo "- libavformat" >> project/README.md
echo "- libswscale" >> project/README.md
echo "- libavutil" >> project/README.md
echo "- GTest" >> project/README.md

echo "Project structure created."
