#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include <opencv2/imgproc/imgproc.hpp>

class GrayscaleImage
{
public:
    GrayscaleImage(const std::string file_name);
    GrayscaleImage(const GrayscaleImage& other);

    int width() const;
    int height() const;
    int pixel_count() const;
    uint8_t pixel(int x, int y) const;
    void pixel(int x, int y, uint8_t val);
    void scale_values(double factor);
    void show(const std::string winName) const;
    void save(const std::string fileName) const;
private:
    cv::Mat _data;
};

#endif
