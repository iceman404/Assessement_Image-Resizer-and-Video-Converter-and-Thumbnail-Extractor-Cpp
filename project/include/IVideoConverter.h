#ifndef IVIDEOCONVERTER_H
#define IVIDEOCONVERTER_H

#include <string>
#include <memory>
extern "C" {
#include <libavutil/frame.h>
}


class IVideoConverter {
public:
    virtual ~IVideoConverter() {}

    // Converts video to MP4 format
    virtual void convertToMP4(const std::string& inputFile, const std::string& outputFile) = 0;

    // Extracts a thumbnail from the video
    virtual AVFrame* extractThumbnail(const std::string& inputFile, int timestamp) = 0;
};

#endif // IVIDEOCONVERTER_H
