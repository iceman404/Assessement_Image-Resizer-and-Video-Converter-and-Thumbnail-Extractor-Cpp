extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/timestamp.h>
}

#include <iostream>
#include <stdexcept>

void convert_webm_to_mp4(const char* input_file, const char* output_file) {
    AVFormatContext *input_ctx = nullptr;
    AVFormatContext *output_ctx = nullptr;
    AVStream *in_stream = nullptr;
    AVStream *out_stream = nullptr;
    AVPacket packet;

    av_register_all();

    // Open input file
    if (avformat_open_input(&input_ctx, input_file, nullptr, nullptr) != 0) {
        throw std::runtime_error("Could not open input file.");
    }

    // Retrieve stream information
    if (avformat_find_stream_info(input_ctx, nullptr) < 0) {
        throw std::runtime_error("Could not find stream information.");
    }

    // Create output format context for MP4
    avformat_alloc_output_context2(&output_ctx, nullptr, "mp4", output_file);

    for (unsigned int i = 0; i < input_ctx->nb_streams; i++) {
        in_stream = input_ctx->streams[i];

        // Create a new stream in the output file
        out_stream = avformat_new_stream(output_ctx, nullptr);
        if (!out_stream) {
            throw std::runtime_error("Failed to allocate output stream.");
        }

        // Determine if the stream is video or audio and copy parameters
        AVCodecParameters *codec_par = in_stream->codecpar;
        if (codec_par->codec_type == AVMEDIA_TYPE_VIDEO) {
            // Copy codec parameters
            avcodec_parameters_copy(out_stream->codecpar, codec_par);
            out_stream->codecpar->codec_tag = 0;  // Important for MP4 format

            // Change codec to H.264 if necessary
            if (codec_par->codec_id != AV_CODEC_ID_H264) {
                // Change codec to H.264
                AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
                if (!codec) {
                    throw std::runtime_error("H.264 codec not found.");
                }

                AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
                if (!codec_ctx) {
                    throw std::runtime_error("Could not allocate codec context.");
                }

                codec_ctx->width = codec_par->width;
                codec_ctx->height = codec_par->height;
                codec_ctx->time_base = in_stream->time_base;
                codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
                codec_ctx->bit_rate = 400000; // Set appropriate bitrate

                if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
                    throw std::runtime_error("Could not open H.264 codec.");
                }

                // Set codec parameters from context
                if (avcodec_parameters_from_context(out_stream->codecpar, codec_ctx) < 0) {
                    throw std::runtime_error("Failed to copy codec parameters.");
                }

                avcodec_free_context(&codec_ctx);
            }
        } else if (codec_par->codec_type == AVMEDIA_TYPE_AUDIO) {
            // Similar process for audio, changing to AAC if necessary
            avcodec_parameters_copy(out_stream->codecpar, codec_par);
            out_stream->codecpar->codec_tag = 0;  // Important for MP4 format

            if (codec_par->codec_id != AV_CODEC_ID_AAC) {
                AVCodec *audio_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
                if (!audio_codec) {
                    throw std::runtime_error("AAC codec not found.");
                }

                AVCodecContext *audio_codec_ctx = avcodec_alloc_context3(audio_codec);
                if (!audio_codec_ctx) {
                    throw std::runtime_error("Could not allocate audio codec context.");
                }

                audio_codec_ctx->sample_rate = codec_par->sample_rate;
                audio_codec_ctx->channel_layout = codec_par->channel_layout;
                audio_codec_ctx->channels = codec_par->channels;
                audio_codec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;

                if (avcodec_open2(audio_codec_ctx, audio_codec, nullptr) < 0) {
                    throw std::runtime_error("Could not open AAC codec.");
                }

                if (avcodec_parameters_from_context(out_stream->codecpar, audio_codec_ctx) < 0) {
                    throw std::runtime_error("Failed to copy audio codec parameters.");
                }

                avcodec_free_context(&audio_codec_ctx);
            }
        }
    }

    // Open the output file and write the header
    if (!(output_ctx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_ctx->pb, output_file, AVIO_FLAG_WRITE) < 0) {
            throw std::runtime_error("Could not open output file.");
        }
    }

    if (avformat_write_header(output_ctx, nullptr) < 0) {
        throw std::runtime_error("Error writing output file header.");
    }

    // Read frames from input and write to output
    while (av_read_frame(input_ctx, &packet) >= 0) {
        in_stream = input_ctx->streams[packet.stream_index];
        out_stream = output_ctx->streams[packet.stream_index];

        // Rescale packet timestamps
        packet.pts = av_rescale_q(packet.pts, in_stream->time_base, out_stream->time_base);
        packet.dts = av_rescale_q(packet.dts, in_stream->time_base, out_stream->time_base);
        packet.duration = av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base);
        packet.pos = -1;

        // Write the packet to the output file
        if (av_interleaved_write_frame(output_ctx, &packet) < 0) {
            std::cerr << "Error muxing packet.\n";
            break;
        }

        av_packet_unref(&packet);
    }

    // Write the output file trailer
    av_write_trailer(output_ctx);

    // Cleanup resources
    avformat_close_input(&input_ctx);
    if (output_ctx && !(output_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&output_ctx->pb);
    }
    avformat_free_context(output_ctx);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_video> <output_mp4>\n";
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    try {
        convert_webm_to_mp4(input_file, output_file);
        std::cout << "Video successfully converted to MP4.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}