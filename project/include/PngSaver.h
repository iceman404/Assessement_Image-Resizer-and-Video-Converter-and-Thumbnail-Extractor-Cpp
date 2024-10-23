#ifndef PNGSAVER_H
#define PNGSAVER_H

#include "IImageSaver.h"
#include <string>

class PngSaver : public IImageSaver {
public:
    virtual ~PngSaver() = default;
    void save(AVFrame* frame, int width, int height, const std::string& outputFilePath) override;
};

#endif // PNGSAVER_H
