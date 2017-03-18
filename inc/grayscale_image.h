#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include <opencv2/imgproc/imgproc.hpp>

class GrayscaleImage
{
public:
    GrayscaleImage(std::string file_name);

    int width() const;
    int height() const;
    int pixel_count() const;
    uint8_t pixel(int x, int y) const;
private:
    cv::Mat _data;
};

#endif
