#ifndef HISTOGRAM_BASE_H
#define HISTOGRAM_BASE_H

#include "grayscale_image.h"

class HistogramBase
{
public:
    static const int size = 256;

    virtual ~HistogramBase() = default;

    virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr) = 0;

    double bin(int bin_idx) const;
    int used_samples() const;
    double total_probabilities() const;
    virtual std::string to_string(bool with_params=false) const = 0;

protected:
    void clear_data();

    int _used_samples;
    double _data[size];
};

#endif
