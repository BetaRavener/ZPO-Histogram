#ifndef RANDOMAREAS_HISTOGRAM_H
#define RANDOMAREAS_HISTOGRAM_H

#include "histogram_base.h"

class RandomAreasHistogram : public HistogramBase
{
public:
    RandomAreasHistogram(int numPixels, int areas);

    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
    virtual std::string to_string(bool with_params=false) const;
private:
    int _numPixels;
    int _areas;
};

#endif
