#ifndef THUMBNAILEXTRACTOR_H
#define THUMBNAILEXTRACTOR_H

#include <string>
extern "C" {
#include <libavutil/frame.h>
}

class ThumbnailExtractor {
public:
    // Saves the thumbnail frame as a JPEG image
    void saveThumbnail(AVFrame* frame, const std::string& outputFile);
};

#endif // THUMBNAILEXTRACTOR_H
