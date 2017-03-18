#ifndef HISTOGRAM_BASE_H
#define HISTOGRAM_BASE_H

#include "grayscale_image.h"

class HistogramBase
{
public:
    static const int size = 256;

    virtual ~HistogramBase() = default;

    virtual void compute(const GrayscaleImage& img) = 0;

    int bin(int bin_idx) const;
    int used_samples() const;
    int samples() const;
    std::string to_string() const;

protected:
    void clear_data();
    virtual void scale_up(int samples_count);

    int _used_samples;
    int _data[size];
};

#endif
