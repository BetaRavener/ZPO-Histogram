#include "undersampled_histogram.h"

UndersampledHistogram::UndersampledHistogram(int step) :
    _step(step)
{
}

void UndersampledHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();

    for (int y = 0; y < img.height(); y += _step)
    {
        for (int x = 0; x < img.width(); x += _step)
        {
            _data[img.pixel(x,y)]++;
            _used_samples++;
            if (mark_img != nullptr)
                mark_img->pixel(x,y, 255);
        }
    }

    // Normalize histogram
    for (int i = 0; i < size; i++)
        _data[i] /= _used_samples;
}

std::string UndersampledHistogram::to_string(bool with_params) const
{
    return "Downsampled Histogram" + (with_params ?
            (" (" + std::to_string(_step) + ")") : "");
}
