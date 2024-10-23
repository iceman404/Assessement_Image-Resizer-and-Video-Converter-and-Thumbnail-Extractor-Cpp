// libav includes
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}



#include "ImageSaverFactory.h" // Ensure this header is included
#include "IImageSaver.h"      // Include IImageSaver if necessary
#include "JpegResizer.h"
#include <iostream>
#include <stdexcept>
#include <fstream>



void JpegResizer::resize(const std::string& inputFilePath, const std::string& outputFilePath, int targetWidth) {
    // Initialize FFmpeg libraries
    //av_register_all();

    AVFormatContext* formatContext = nullptr;
    AVPacket pkt;
    if (avformat_open_input(&formatContext, inputFilePath.c_str(), nullptr, nullptr) < 0) {
        throw std::runtime_error("Could not open input file.");
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("Could not find stream information.");
    }

    AVCodec* codec = nullptr;
    AVCodecParameters* codecParameters = nullptr;
    int videoStreamIndex = -1;

    for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            codecParameters = formatContext->streams[i]->codecpar;
            codec = avcodec_find_decoder(codecParameters->codec_id);
            break;
        }
    }

    if (videoStreamIndex == -1) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("Could not find a video stream.");
    }

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("Could not allocate codec context.");
    }

    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Could not copy codec parameters to context.");
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Could not open codec.");
    }

    AVFrame* frame = av_frame_alloc();
    AVFrame* resizedFrame = av_frame_alloc();
    if (!frame || !resizedFrame) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Could not allocate frames.");
    }

    // Allocate buffer for resized image
    int newHeight = static_cast<int>(codecContext->height * static_cast<double>(targetWidth) / codecContext->width);
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, targetWidth, newHeight, 32);
    uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(resizedFrame->data, resizedFrame->linesize, buffer, AV_PIX_FMT_RGB24, targetWidth, newHeight, 32);

    // Read frames and resize
    int response;
    while (av_read_frame(formatContext, &pkt) >= 0) {
        if (pkt.stream_index == videoStreamIndex) {
            response = avcodec_send_packet(codecContext, &pkt);
            if (response < 0) {
                av_packet_unref(&pkt);
                break; // Error in sending packet
            }

            while (response >= 0) {
                response = avcodec_receive_frame(codecContext, frame);
                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                    av_packet_unref(&pkt);
                    break; // No more frames
                } else if (response < 0) {
                    av_packet_unref(&pkt);
                    throw std::runtime_error("Error during decoding.");
                }

                // Resize frame
                SwsContext* swsCtx = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
                                                     targetWidth, newHeight, AV_PIX_FMT_RGB24, SWS_BILINEAR,
                                                     nullptr, nullptr, nullptr);
                sws_scale(swsCtx, frame->data, frame->linesize, 0, codecContext->height, resizedFrame->data, resizedFrame->linesize);
                sws_freeContext(swsCtx);
            }
        }
        av_packet_unref(&pkt);
    }

    // Save the resized image
    // Assuming an ImageSaver interface has been implemented, use it here
    auto imageSaver = ImageSaverFactory::createImageSaver("jpeg"); // Keeps it as a unique_ptr

    if (!imageSaver) {
        av_freep(&buffer);
        av_frame_free(&resizedFrame);
        av_frame_free(&frame);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Invalid image saver type.");
    }

    imageSaver->save(resizedFrame, targetWidth, newHeight, outputFilePath);


    // Clean up
    av_freep(&buffer);
    av_frame_free(&resizedFrame);
    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}
