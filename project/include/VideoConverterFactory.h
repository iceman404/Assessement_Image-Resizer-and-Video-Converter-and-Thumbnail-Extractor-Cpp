#ifndef VIDEOCONVERTERFACTORY_H
#define VIDEOCONVERTERFACTORY_H

#include "IVideoConverter.h"
#include <string>
#include <memory>

class VideoConverterFactory {
public:
    // Factory method to create the appropriate video converter
    std::unique_ptr<IVideoConverter> createConverter(const std::string& inputFile);
};

#endif // VIDEOCONVERTERFACTORY_H
