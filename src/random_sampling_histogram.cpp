#include "random_sampling_histogram.h"
#include <random>
#include <iostream>
#include <functional>

RandomSamplingHistogram::RandomSamplingHistogram(int numPixels) :
    _numPixels(numPixels)
{
}

void RandomSamplingHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();

    int imgNumPixels = img.pixel_count();
    int width = img.width();
    std::cout << "width: " << width << " height: " << img.height() << std::endl;
    int pixel; // 1D pixel address
    int x, y;

    //std::default_random_engine generator;
    //std::uniform_int_distribution<int> distribution(0, imgNumPixels - 1);
    std::mt19937::result_type seed = time(0);
    auto rand_num = std::bind(std::uniform_int_distribution<int>(0, imgNumPixels - 1), std::mt19937(seed));

    for(int i = 0; i < _numPixels; i++)
    {
        //pixel = distribution(generator);
        pixel = rand_num();
        y = pixel / width;
        x = pixel % width;
            if (x > width || y > img.height())
                std::cout << "x: " << x << " y: " << y << std::endl;
        
        _used_samples++;
        _data[img.pixel(x,y)]++;
        if (mark_img != nullptr)
            mark_img->pixel(x,y, 255);

    }

     // Normalize histogram
    for (int i = 0; i < size; i++)
        _data[i] /= _used_samples;
}

std::string RandomSamplingHistogram::to_string(bool with_params) const
{
    return "Downsampled Histogram" + (with_params ?
            (" (" + std::to_string(_numPixels) + ")") : "");
}
