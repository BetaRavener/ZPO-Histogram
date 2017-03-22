#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "histogram_base.h"

class Evaluator
{
public:
    static double sum_of_squared_differences(const HistogramBase& hist_a,
            const HistogramBase& hist_b);
    static void compare_histrograms_text(const HistogramBase& hist_a,
            const HistogramBase& hist_b);
    
private:
};

#endif
