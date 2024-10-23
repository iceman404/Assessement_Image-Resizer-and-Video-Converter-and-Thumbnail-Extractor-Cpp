#ifndef PNGRESIZER_H
#define PNGRESIZER_H

#include "IImageResizer.h"

class PngResizer : public IImageResizer {
public:
    void resize(const std::string& inputFilePath, const std::string& outputFilePath, int targetWidth) override;
};

#endif // PNGRESIZER_H
