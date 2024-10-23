#include "ThumbnailExtractor.h"
#include <iostream>
// libav includes
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

void ThumbnailExtractor::saveThumbnail(AVFrame* frame, const std::string& outputFile) {
    std::cout << "Saving thumbnail to: " << outputFile << std::endl;

    // Save thumbnail as JPEG or PNG using libav (not fully implemented here)
    // avcodec_encode_image, av_frame_make_writable etc.
    // ...
}
