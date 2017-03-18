#include "histogram_base.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

int HistogramBase::bin(int bin_idx) const
{
    if (bin_idx < 0 || bin_idx >= size)
        throw std::range_error("Bin index " + std::to_string(bin_idx) + " out of range");

    return _data[bin_idx];
}

int HistogramBase::used_samples() const
{
    return _used_samples;
}

int HistogramBase::samples() const
{
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += _data[i];
    return sum;
}

std::string HistogramBase::to_string() const
{
    std::ostringstream oss;
    for (int i = 0; i < size; i++)
        oss << i << ": " << _data[i] << "\n";
    return oss.str();
}

void HistogramBase::clear_data()
{
    _used_samples = 0;
    for (int i = 0; i < size; i++)
        _data[i] = 0;
}

void HistogramBase::scale_up(int samples_count)
{
    float scale_factor = (float)samples_count / _used_samples;

    float scaled_histo[size];
    int floored_samples = 0;
    // Scale up the histogram by the factor.
    // Count the floored samples to find how many
    // values may be rounded up.
    for (int i = 0; i < size; i++)
    {
        scaled_histo[i] = _data[i] * scale_factor;
        floored_samples += scaled_histo[i];
    }

    // Distribute the leftover samples to bins
    // with highest decimal part
    int leftover_count = samples_count - floored_samples;
    if (leftover_count < 0 || leftover_count >= size)
        throw std::runtime_error("Error in scale up algorithm, this shouldn't happen");
    while (leftover_count-- > 0)
    {
        // Find bin with highest decimal part
        int max_idx = -1;
        float max_decimal = -1;
        for (int i = 0; i < size; i++)
        {
            float decimal = scaled_histo[i] - (int)scaled_histo[i];
            if (decimal > max_decimal)
            {
                max_idx = i;
                max_decimal = decimal;
            }
        }
        scaled_histo[max_idx] = (int)(scaled_histo[max_idx] + 1);
    }

    // Rewrite histogram data
    for (int i = 0; i < size; i++)
        _data[i] = (int)scaled_histo[i];
}
