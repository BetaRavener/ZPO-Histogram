#ifndef NEARESTNEIGHBOUR_HISTOGRAM_H
#define NEARESTNEIGHBOUR_HISTOGRAM_H

#include "histogram_base.h"

class NearestNeighbourHistogram : public HistogramBase
{
public:
    NearestNeighbourHistogram(int numPixels);

    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
    virtual std::string to_string(bool with_params=false) const;
private:
    int _numPixels;
};

#endif
