#ifndef UNDERSAMPLED_HISTOGRAM_H
#define UNDERSAMPLED_HISTOGRAM_H

#include "histogram_base.h"

class UndersampledHistogram : public HistogramBase
{
public:
    UndersampledHistogram(int skip=2);
    virtual ~UndersampledHistogram() = default;

    virtual void compute(const GrayscaleImage& img);
private:
    int _skip;
};

#endif
