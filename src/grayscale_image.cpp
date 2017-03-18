#include "grayscale_image.h"

#include <stdexcept>

#include <opencv2/highgui/highgui.hpp>

GrayscaleImage::GrayscaleImage(std::string file_name)
{
    cv::Mat src_rgb = cv::imread(file_name);
    if( src_rgb.empty() ) {
        throw std::runtime_error("Failed to load image: " + file_name);
    }
    cvtColor(src_rgb, _data, CV_BGR2GRAY);
}

int GrayscaleImage::width() const
{
    return _data.cols;
}

int GrayscaleImage::height() const
{
    return _data.rows;
}

uint8_t GrayscaleImage::pixel(int x, int y) const
{
    return _data.at<uchar>(y, x);
}

int GrayscaleImage::pixel_count() const
{
    return width() * height();
}
