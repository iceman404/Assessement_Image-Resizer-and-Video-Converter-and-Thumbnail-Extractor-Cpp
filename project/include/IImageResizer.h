#ifndef IIMAGERESIZER_H
#define IIMAGERESIZER_H

#include <string>

class IImageResizer {
public:
    virtual ~IImageResizer() = default;

    // Resize image to the specified width while preserving aspect ratio
    virtual void resize(const std::string& inputFilePath, const std::string& outputFilePath, int targetWidth) = 0;
};

#endif // IIMAGERESIZER_H
