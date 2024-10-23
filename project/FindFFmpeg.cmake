# FindFFmpeg.cmake
find_path(FFMPEG_INCLUDE_DIR
    NAMES avcodec.h avformat.h avutil.h swscale.h
    PATHS /usr/include /usr/local/include
)

find_library(FFMPEG_LIBRARIES
    NAMES avcodec avformat avutil swscale
    PATHS /usr/lib /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFmpeg DEFAULT_MSG FFMPEG_LIBRARIES FFMPEG_INCLUDE_DIR)

mark_as_advanced(FFMPEG_INCLUDE_DIR FFMPEG_LIBRARIES)
