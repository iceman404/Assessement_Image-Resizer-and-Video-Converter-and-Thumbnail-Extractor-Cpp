#ifndef IIMAGESAVER_H
#define IIMAGESAVER_H

#include <string>
extern "C" {
#include <libavutil/frame.h>
}


class IImageSaver {
public:
    virtual ~IImageSaver() = default;

    // Pure virtual function to be implemented by subclasses
    virtual void save(AVFrame* frame, int width, int height, const std::string& outputFilePath) = 0;
};

#endif // IIMAGESAVER_H
