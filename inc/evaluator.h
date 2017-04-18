#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory>
#include <vector>

#include "histogram_base.h"
#include "full_histogram.h"

class ExperimentResult
{
public:
    double min_SSD, max_SSD, median_SSD;
    double min_score, max_score, median_score;
};

union ParamType
{
    int i;
    double d;
};

// Forward declaration
class Experiment;

class Evaluator
{
public:
    enum Methods {
        Downsample,
        Cross,
        Circle,
        Chess,
        Masked
    };

    static std::unique_ptr<HistogramBase> make_class(Methods method,
            const std::vector<ParamType>& params);
    static double score(const HistogramBase& hist_a,
                const HistogramBase& hist_b);
    static double used_pixel_ratio(const HistogramBase& hist_a,
                const HistogramBase& hist_b);
    static double sum_of_squared_differences(const HistogramBase& hist_a,
            const HistogramBase& hist_b);
    static double median(std::vector<double> vec);
    static void compare_histrograms_text(const HistogramBase& hist_a,
            const HistogramBase& hist_b);
    static void do_experiment(Experiment& experiment,
            const std::vector<std::string>& filenames,
            const std::vector<FullHistogram>& full_histograms);
private:
};

class Experiment
{
public:
    Evaluator::Methods method;
    std::vector<std::vector<ParamType>> params;
    std::vector<ExperimentResult> results;
};

#endif
