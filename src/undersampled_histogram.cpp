#include "undersampled_histogram.h"

UndersampledHistogram::UndersampledHistogram(int skip) :
    _skip(skip)
{
}

void UndersampledHistogram::compute(const GrayscaleImage& img)
{
    clear_data();

    for (int y = 0; y < img.height(); y += _skip)
    {
        for (int x = 0; x < img.width(); x += _skip)
        {
            _data[img.pixel(x,y)]++;
            _used_samples++;
        }
    }

    scale_up(img.pixel_count());
}
