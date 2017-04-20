#include "nearest_neighbour_histogram.h"
#include <random>
#include <iostream>
#include <functional>

NearestNeighbourHistogram::NearestNeighbourHistogram(int pixelPercent) :
    _pixelPercent(pixelPercent)
{
}

void NearestNeighbourHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();

    int width = img.width();
    int height = img.height();
    int numPixels = (width * height * _pixelPercent) / 100;
    int x, y;
    std::vector<int> hodnoty;

    double aspect = (double)width / (double)height;
    
    int newHeight =(int)llround(sqrt(numPixels/aspect));
    int newWidth = (int)llround(aspect * sqrt(numPixels/aspect));

    double x_ratio = (double)width/(double)newWidth;
    double y_ratio = (double)height/(double)newHeight;
    for (int dimY = 0; dimY < newHeight; dimY++)
    {
        y = (int)floor(dimY * y_ratio);
        for (int dimX = 0; dimX < newWidth; dimX++)
        {
            x = (int)floor(dimX * x_ratio);
            _used_samples++;
            _data[img.pixel(x,y)]++;
            if (mark_img != nullptr)
                mark_img->pixel(x,y, 255);
        }
    }

     // Normalize histogram
    for (int i = 0; i < size; i++)
        _data[i] /= _used_samples;
}

std::string NearestNeighbourHistogram::to_string(bool with_params) const
{
    return "Nearest Neighbour" + (with_params ?
            (" (" + std::to_string(_pixelPercent) + ")") : "");
}
