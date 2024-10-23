#ifndef JPEGRESIZER_H
#define JPEGRESIZER_H

#include "IImageResizer.h"

class JpegResizer : public IImageResizer {
public:
    void resize(const std::string& inputFilePath, const std::string& outputFilePath, int targetWidth) override;
};

#endif // JPEGRESIZER_H
