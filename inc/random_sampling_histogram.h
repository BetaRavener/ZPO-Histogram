#ifndef RANDOMSAMPLING_HISTOGRAM_H
#define RANDOMSAMPLING_HISTOGRAM_H

#include "histogram_base.h"

class RandomSamplingHistogram : public HistogramBase
{
public:
    RandomSamplingHistogram(int numPixels);

    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
    virtual std::string to_string(bool with_params=false) const;
private:
    int _pixelPercent;
};

#endif
