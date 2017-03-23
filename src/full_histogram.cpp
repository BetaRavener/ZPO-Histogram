#include "full_histogram.h"

void FullHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();

    for (int y = 0; y < img.height(); y++)
    {
        for (int x = 0; x < img.width(); x++)
        {
            _data[img.pixel(x,y)]++;
            _used_samples++;
            if (mark_img != nullptr)
                mark_img->pixel(x,y, 255);
        }
    }

    // Normalize each bin
    for (int i = 0; i < size; i++)
        _data[i] /= _used_samples;
}

std::string FullHistogram::to_string(bool with_params) const
{
    return "Full Histogram";
}
