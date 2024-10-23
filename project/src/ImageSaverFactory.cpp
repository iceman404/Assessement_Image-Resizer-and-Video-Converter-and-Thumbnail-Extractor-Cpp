#include "ImageSaverFactory.h"
#include "JpegSaver.h"
#include "PngSaver.h"
#include <stdexcept>

std::unique_ptr<IImageSaver> ImageSaverFactory::createImageSaver(const std::string& format) {
    if (format == "jpeg" || format == "jpg") {
        return std::make_unique<JpegSaver>();
    } else if (format == "png") {
        return std::make_unique<PngSaver>();
    } else {
        throw std::invalid_argument("Unsupported image format: " + format);
    }
}
