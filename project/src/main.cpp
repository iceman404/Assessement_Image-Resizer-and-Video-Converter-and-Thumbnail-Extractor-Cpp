#include "ImageResizerFactory.h"
#include "VideoConverterFactory.h"
#include "ThumbnailExtractor.h"
#include "ImageSaverFactory.h"
#include <iostream>
#include <memory>


#define SMALL_WIDTH 250
#define MEDIUM_WIDTH 350
#define LARGE_WIDTH 650

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <image_format> <input_file> <output_file>\n";
        return 1;
    }

    std::string format = argv[1];
    std::string inputFilePath = argv[2];
    std::string outputFilePath = argv[3];

    try {
        auto resizer = ImageResizerFactory::createResizer(format);

        // Here we can select a target width, for example, SMALL_WIDTH
        int targetWidth = MEDIUM_WIDTH;
        resizer->resize(inputFilePath, outputFilePath, targetWidth);

        std::cout << "Image resized successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}



/*
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_video_file>" << std::endl;
        return 1;
    }

    std::string inputVideo = argv[1];
    std::string outputMP4 = "converted_video.mp4";
    std::string thumbnailSmall = "thumbnail_small.jpg";
    std::string thumbnailMedium = "thumbnail_medium.jpg";
    std::string thumbnailLarge = "thumbnail_large.jpg";

    // Create video converter using factory
    VideoConverterFactory videoFactory;
    auto converter = videoFactory.createConverter(inputVideo);
    if (!converter) {
        return 1;
    }

    // Convert video to MP4
    converter->convertToMP4(inputVideo, outputMP4);

    // Extract thumbnail from the video at 5 seconds
    AVFrame* thumbnail = converter->extractThumbnail(inputVideo, 5);
    if (!thumbnail) {
        std::cerr << "Failed to extract thumbnail" << std::endl;
        return 1;
    }

    // Create thumbnail extractor to save resized thumbnails
    ThumbnailExtractor extractor;
    ImageResizerFactory resizerFactory;

    // Resize and save the thumbnails
    auto resizerSmall = resizerFactory.createResizer(thumbnailSmall);
    resizerSmall->resizeImage(thumbnailSmall, thumbnailSmall, 250);
    extractor.saveThumbnail(thumbnail, thumbnailSmall);

    auto resizerMedium = resizerFactory.createResizer(thumbnailMedium);
    resizerMedium->resizeImage(thumbnailMedium, thumbnailMedium, 350);
    extractor.saveThumbnail(thumbnail, thumbnailMedium);

    auto resizerLarge = resizerFactory.createResizer(thumbnailLarge);
    resizerLarge->resizeImage(thumbnailLarge, thumbnailLarge, 650);
    extractor.saveThumbnail(thumbnail, thumbnailLarge);

    std::cout << "Video conversion and thumbnail extraction complete." << std::endl;
    return 0;
}
*/