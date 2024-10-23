#ifndef IMAGESAVERFACTORY_H
#define IMAGESAVERFACTORY_H

#include "IImageSaver.h"
#include <memory>
#include <string>

class ImageSaverFactory {
public:
    static std::unique_ptr<IImageSaver> createImageSaver(const std::string& format);
};

#endif // IMAGESAVERFACTORY_H
