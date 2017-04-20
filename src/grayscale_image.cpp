#include "grayscale_image.h"

#include <stdexcept>

#include <opencv2/highgui/highgui.hpp>

GrayscaleImage::GrayscaleImage(const std::string file_name)
{
    cv::Mat src_rgb = cv::imread(file_name);
    if( src_rgb.empty() ) {
        throw std::runtime_error("Failed to load image: " + file_name);
    }
    cvtColor(src_rgb, _data, CV_BGR2GRAY);
}

GrayscaleImage::GrayscaleImage(const GrayscaleImage& other)
{
    _data = other._data.clone();
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
    if (x < 0 || x >= _data.cols || y < 0 || y >= _data.rows)
        throw std::range_error("Pixel out of image range.");

    return _data.at<uchar>(y, x);
}

int GrayscaleImage::pixel_count() const
{
    return width() * height();
}

void GrayscaleImage::pixel(int x, int y, uint8_t val)
{
    if (x < 0 || x >= _data.cols || y < 0 || y >= _data.rows)
        throw std::range_error("Pixel out of image range.");
    _data.at<uchar>(y, x) = val;
}

void GrayscaleImage::scale_values(double factor)
{
    for (int y = 0; y < _data.rows; y++)
    {
        for (int x = 0; x < _data.cols; x++)
        {
            double scaled = _data.at<uchar>(y, x) * factor;
            _data.at<uchar>(y, x) = cv::saturate_cast<uchar>(scaled);
        }
    }
}

void GrayscaleImage::show(const std::string winName) const
{
    cv::imshow(winName, _data);
}

void GrayscaleImage::save(const std::string fileName) const
{
    cv::imwrite(fileName, _data);
}
