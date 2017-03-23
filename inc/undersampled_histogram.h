#ifndef UNDERSAMPLED_HISTOGRAM_H
#define UNDERSAMPLED_HISTOGRAM_H

#include "histogram_base.h"

class UndersampledHistogram : public HistogramBase
{
public:
    UndersampledHistogram(int step=2);
    virtual ~UndersampledHistogram() = default;

    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
    virtual std::string to_string(bool with_params=false) const;
private:
    int _step;
};

#endif
