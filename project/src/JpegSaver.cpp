
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}



#include "JpegSaver.h"
#include <stdexcept>
#include <fstream>


void JpegSaver::save(AVFrame* frame, int width, int height, const std::string& outputFilePath) {
    const AVCodec* jpegCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
    if (!jpegCodec) {
        throw std::runtime_error("MJPEG codec not found.");
    }

    AVCodecContext* jpegCtx = avcodec_alloc_context3(jpegCodec);
    if (!jpegCtx) {
        throw std::runtime_error("Failed to allocate codec context for JPEG.");
    }

    jpegCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
    jpegCtx->width = width;
    jpegCtx->height = height;

    if (avcodec_open2(jpegCtx, jpegCodec, nullptr) < 0) {
        avcodec_free_context(&jpegCtx);
        throw std::runtime_error("Failed to open JPEG codec.");
    }

    AVPacket* packet = av_packet_alloc();
    if (!packet) {
        avcodec_free_context(&jpegCtx);
        throw std::runtime_error("Failed to allocate AVPacket.");
    }

    int response = avcodec_send_frame(jpegCtx, frame);
    if (response < 0) {
        av_packet_free(&packet);
        avcodec_free_context(&jpegCtx);
        throw std::runtime_error("Failed to send frame to encoder.");
    }

    response = avcodec_receive_packet(jpegCtx, packet);
    if (response < 0) {
        av_packet_free(&packet);
        avcodec_free_context(&jpegCtx);
        throw std::runtime_error("Failed to receive packet from encoder.");
    }

    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        av_packet_free(&packet);
        avcodec_free_context(&jpegCtx);
        throw std::runtime_error("Failed to open output file.");
    }

    outFile.write(reinterpret_cast<const char*>(packet->data), packet->size);
    outFile.close();

    av_packet_free(&packet);
    avcodec_free_context(&jpegCtx);
}
