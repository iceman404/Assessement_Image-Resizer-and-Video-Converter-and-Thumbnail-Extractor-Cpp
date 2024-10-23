#include "VideoConverterFactory.h"
#include "WebMConverter.h"
#include <iostream>
#include <memory>

std::unique_ptr<IVideoConverter> VideoConverterFactory::createConverter(const std::string& inputFile) {
    // Determine file type based on extension (simplified)
    if (inputFile.find(".webm") != std::string::npos) {
        return std::make_unique<WebMConverter>();
    } else {
        std::cerr << "Unsupported video format: " << inputFile << std::endl;
        return nullptr;
    }
}
