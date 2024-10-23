#ifndef IMAGERESIZERFACTORY_H
#define IMAGERESIZERFACTORY_H

#include "IImageResizer.h"
#include <memory>
#include <stdexcept>

class ImageResizerFactory {
public:
    static std::unique_ptr<IImageResizer> createResizer(const std::string& format);
};

#endif // IMAGERESIZERFACTORY_H
