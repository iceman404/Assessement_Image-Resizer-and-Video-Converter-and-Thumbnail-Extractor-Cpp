#ifndef WEBMCONVERTER_H
#define WEBMCONVERTER_H

#include "IVideoConverter.h"

class WebMConverter : public IVideoConverter {
public:
    virtual ~WebMConverter() {}

    // Convert WebM to MP4
    void convertToMP4(const std::string& inputFile, const std::string& outputFile) override;

    // Extracts a thumbnail from WebM
    AVFrame* extractThumbnail(const std::string& inputFile, int timestamp) override;
};

#endif // WEBMCONVERTER_H
