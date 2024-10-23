// libav includes
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include "PngResizer.h"
#include <iostream>


void PngResizer::resize(const std::string& inputFilePath, const std::string& outputFilePath, int targetWidth) {
    // Initialize libav
    //av_register_all();
    
    // Open the input file
    AVFormatContext* pFormatCtx = nullptr;
    if (avformat_open_input(&pFormatCtx, inputFilePath.c_str(), nullptr, nullptr) < 0) {
        throw std::runtime_error("Could not open input file: " + inputFilePath);
    }
    
    // Get the first video stream
    AVCodecParameters* pCodecParameters = pFormatCtx->streams[0]->codecpar;
    int originalWidth = pCodecParameters->width;
    int originalHeight = pCodecParameters->height;

    // Calculate the new height preserving aspect ratio
    int targetHeight = (originalHeight * targetWidth) / originalWidth;

    // Resize logic using libav goes here...

    // Clean up
    avformat_close_input(&pFormatCtx);
}
