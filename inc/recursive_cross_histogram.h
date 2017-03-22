#ifndef RECURSIVE_CROSS_HISTOGRAM_H
#define RECURSIVE_CROSS_HISTOGRAM_H

#include <stdint.h>

#include "histogram_base.h"

class RecursiveCrossHistogram : public HistogramBase
{
public:
    RecursiveCrossHistogram(int threshold, uint64_t forced_division_area);
    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
    virtual std::string to_string() const;
private:
    int _threshold;
    uint64_t _forced_division_area;
};

#endif
