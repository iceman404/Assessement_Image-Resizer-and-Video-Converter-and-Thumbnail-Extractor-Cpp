#ifndef JPEGSAVER_H
#define JPEGSAVER_H

#include "IImageSaver.h"
#include <string>

class JpegSaver : public IImageSaver {
public:
    virtual ~JpegSaver() = default;
    void save(AVFrame* frame, int width, int height, const std::string& outputFilePath) override;
};

#endif // JPEGSAVER_H
