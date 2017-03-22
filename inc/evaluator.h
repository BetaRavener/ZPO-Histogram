#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "histogram_base.h"

class Evaluator
{
public:
    static double score(const HistogramBase& hist_a,
                const HistogramBase& hist_b);
    static double used_pixel_ratio(const HistogramBase& hist_a,
                const HistogramBase& hist_b);
    static double sum_of_squared_differences(const HistogramBase& hist_a,
            const HistogramBase& hist_b);
    static double median(std::vector<double> vec);
    static void compare_histrograms_text(const HistogramBase& hist_a,
            const HistogramBase& hist_b);
    
private:
};

#endif
