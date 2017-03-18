#include "full_histogram.h"

void FullHistogram::compute(const GrayscaleImage& img)
{
    clear_data();

    for (int y = 0; y < img.height(); y++)
    {
        for (int x = 0; x < img.width(); x++)
        {
            _data[img.pixel(x,y)]++;
            _used_samples++;
        }
    }
}
