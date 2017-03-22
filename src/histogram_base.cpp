#include "histogram_base.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

double HistogramBase::bin(int bin_idx) const
{
    if (bin_idx < 0 || bin_idx >= size)
        throw std::range_error("Bin index " + std::to_string(bin_idx) + " out of range");

    return _data[bin_idx];
}

int HistogramBase::used_samples() const
{
    return _used_samples;
}

double HistogramBase::total_probabilities() const
{
    double acc = 0;
    for (int i = 0; i < size; i++)
        acc += _data[i];
    return acc;
}

void HistogramBase::clear_data()
{
    _used_samples = 0;
    for (int i = 0; i < size; i++)
        _data[i] = 0.0;
}
