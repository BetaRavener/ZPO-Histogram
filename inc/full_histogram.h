#ifndef FULL_HISTOGRAM_H
#define FULL_HISTOGRAM_H

#include "histogram_base.h"

class FullHistogram : public HistogramBase
{
public:
    virtual ~FullHistogram() = default;

    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
};

#endif
