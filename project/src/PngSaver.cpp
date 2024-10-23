extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}



#include "PngSaver.h"
#include <stdexcept>
#include <fstream>


void PngSaver::save(AVFrame* frame, int width, int height, const std::string& outputFilePath) {
    const AVCodec* pngCodec = avcodec_find_encoder(AV_CODEC_ID_PNG);
    if (!pngCodec) {
        throw std::runtime_error("PNG codec not found.");
    }

    AVCodecContext* pngCtx = avcodec_alloc_context3(pngCodec);
    if (!pngCtx) {
        throw std::runtime_error("Failed to allocate codec context for PNG.");
    }

    pngCtx->pix_fmt = AV_PIX_FMT_RGBA;
    pngCtx->width = width;
    pngCtx->height = height;

    if (avcodec_open2(pngCtx, pngCodec, nullptr) < 0) {
        avcodec_free_context(&pngCtx);
        throw std::runtime_error("Failed to open PNG codec.");
    }

    AVPacket* packet = av_packet_alloc();
    if (!packet) {
        avcodec_free_context(&pngCtx);
        throw std::runtime_error("Failed to allocate AVPacket.");
    }

    int response = avcodec_send_frame(pngCtx, frame);
    if (response < 0) {
        av_packet_free(&packet);
        avcodec_free_context(&pngCtx);
        throw std::runtime_error("Failed to send frame to encoder.");
    }

    response = avcodec_receive_packet(pngCtx, packet);
    if (response < 0) {
        av_packet_free(&packet);
        avcodec_free_context(&pngCtx);
        throw std::runtime_error("Failed to receive packet from encoder.");
    }

    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        av_packet_free(&packet);
        avcodec_free_context(&pngCtx);
        throw std::runtime_error("Failed to open output file.");
    }

    outFile.write(reinterpret_cast<const char*>(packet->data), packet->size);
    outFile.close();

    av_packet_free(&packet);
    avcodec_free_context(&pngCtx);
}
