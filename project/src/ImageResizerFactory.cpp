#include "ImageResizerFactory.h"
#include "JpegResizer.h"
#include "PngResizer.h"
#include <iostream>

std::unique_ptr<IImageResizer> ImageResizerFactory::createResizer(const std::string& inputFile) {
    // Determine file type based on extension (simplified)
    if (inputFile.find(".jpg") != std::string::npos || inputFile.find(".jpeg") != std::string::npos) {
        return std::make_unique<JpegResizer>();
    } else if (inputFile.find(".png") != std::string::npos) {
        return std::make_unique<PngResizer>();
    } else {
        std::cerr << "Unsupported image format: " << inputFile << std::endl;
        return nullptr;
    }
}
