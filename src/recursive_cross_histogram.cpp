#include "recursive_cross_histogram.h"

#include <deque>
#include <cmath>

struct Rectangle
{
    Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
    {
        this->x = x;
        this->y = y;
        width = w;
        height = h;
    }

    uint64_t area()
    {
        return (uint64_t)width * height;
    }

    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

RecursiveCrossHistogram::RecursiveCrossHistogram(int threshold, uint64_t forced_division_area) :
    _threshold(threshold),
    _forced_division_area(forced_division_area)
{
}

void RecursiveCrossHistogram::compute(const GrayscaleImage& img,
        GrayscaleImage* mark_img)
{
    clear_data();

    std::deque<Rectangle> rectangles;
    rectangles.push_back(Rectangle(0,0,img.width(),img.height()));
    double sub_hist[HistogramBase::size];

    while (rectangles.size())
    {
        Rectangle current = rectangles.front();
        rectangles.pop_front();
        auto half_w = current.width / 2;
        auto half_h = current.height / 2;
        auto area = current.area();
        if (area == 0)
            continue;

        bool divide = true;
        bool sample_cross = _forced_division_area == 0 || area < _forced_division_area;

        uint8_t min_bin=HistogramBase::size-1, max_bin=0;
        if (sample_cross)
        {
            // Clear sub-histogram
            for (int i = 0; i < HistogramBase::size; i++)
                sub_hist[i] = 0;

            // Sample values in the middle row & column
            uint32_t x, y;
            y = current.y + half_h;
            for (uint32_t cx = 0; cx < current.width; cx++)
            {
                x = current.x + cx;
                auto bin = img.pixel(x, y);
                min_bin = std::min(min_bin, bin);
                max_bin = std::max(max_bin, bin);
                sub_hist[bin]++;
                _used_samples++;
                if (mark_img != nullptr)
                    mark_img->pixel(x, y, 255);
            }

            x = current.x + half_w;
            for (uint32_t cy = 0; cy < current.height; cy++)
            {
                /// Skip cross middle so its not sampled twice
                if (cy == half_h)
                    continue;

                y = current.y + cy;
                auto bin = img.pixel(x, y);
                min_bin = std::min(min_bin, bin);
                max_bin = std::max(max_bin, bin);
                sub_hist[bin]++;
                _used_samples++;
                if (mark_img != nullptr)
                    mark_img->pixel(x, y, 255);
            }

            divide = (max_bin - min_bin) >= _threshold;
        }

        if (sample_cross)
        {
            // End dividing - aggregate sub-histogram scaled to whole area
            if (!divide)
            {
                // Normalize for area it was computed for
                auto norm = (double)area / (current.width + current.height - 1);
                for (int i = 0; i < HistogramBase::size; i++)
                    _data[i] += sub_hist[i] * norm;
                continue;
            }
            else
            {
                // Continue dividing - aggregate only cross histogram
                for (int i = 0; i < HistogramBase::size; i++)
                    _data[i] += sub_hist[i];
            }
        }

        if (!divide || area == 1)
            continue;

        // Divide rectangle into 4 parts
        uint32_t d = sample_cross ? 1 : 0;
        uint32_t xl = current.x, yl = current.y;
        uint32_t wl = half_w, hl = half_h;
        // Skip the row and column that created cross
        uint32_t xr = current.x + wl + d, yr = current.y + hl + d;
        uint32_t wr = current.width - (wl + d), hr = current.height - (hl + d);
        rectangles.push_back(Rectangle(xl, yl, wl, hl));
        if (wr > 0)
            rectangles.push_back(Rectangle(xr, yl, wr, hl));
        if (hr > 0)
            rectangles.push_back(Rectangle(xl, yr, wl, hr));
        if (wr > 0 && hr > 0)
            rectangles.push_back(Rectangle(xr, yr, wr, hr));
    }

    // Normalize the final histogram
    long area = img.width() * img.height();
    for (int i = 0; i < HistogramBase::size; i++)
        _data[i] /= area;
}

std::string RecursiveCrossHistogram::to_string(bool with_params) const
{
    return "Cross Histrogram" +
            (with_params ? (" (" + std::to_string(_threshold) + ", " +
            std::to_string(_forced_division_area) + ")") : "");
}
