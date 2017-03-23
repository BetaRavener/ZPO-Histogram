#include "evaluator.h"

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "undersampled_histogram.h"
#include "recursive_cross_histogram.h"

std::unique_ptr<HistogramBase> Evaluator::make_class(Methods method,
        const std::vector<ParamType>& params)
{
    switch (method)
    {
    case Methods::Downsample: {
        int step = params.size() > 0 ? params[0].i : 2;

        return std::unique_ptr<HistogramBase>(new
                UndersampledHistogram(step));
    }
    case Methods::Cross: {
        int threshold = params.size() > 0 ? params[0].i : 10;
        int forced_division_area = params.size() > 1 ? params[1].i : 0;

        return std::unique_ptr<HistogramBase>(new
                RecursiveCrossHistogram(threshold, forced_division_area));
    }
    default:
        throw std::invalid_argument("Invalid method.");
    }
}

double Evaluator::score(const HistogramBase& hist_a,
        const HistogramBase& hist_b)
{
    double ssd = sum_of_squared_differences(hist_a, hist_b);
    double ratio = used_pixel_ratio(hist_a, hist_b);
    return 1e-6/(ssd*ratio);
}

double Evaluator::used_pixel_ratio(const HistogramBase& hist_a,
    const HistogramBase& hist_b)
{
    return (double)hist_b.used_samples() / hist_a.used_samples();
}

double Evaluator::sum_of_squared_differences(const HistogramBase& hist_a,
        const HistogramBase& hist_b)
{
    double sum = 0;
    for (int i = 0; i < HistogramBase::size; i++)
    {
        double diff = hist_a.bin(i) - hist_b.bin(i);
        sum += diff * diff;
    }
    return sum;
}

double Evaluator::median(std::vector<double> vec)
{
    double median;
    size_t size = vec.size();

    sort(vec.begin(), vec.end());

    if (size  % 2 == 0)
    {
      median = (vec[size / 2 - 1] + vec[size / 2]) / 2;
    }
    else
    {
      median = vec[size / 2];
    }

    return median;
}

void Evaluator::compare_histrograms_text(const HistogramBase& hist_a,
        const HistogramBase& hist_b)
{
    int used_a = hist_a.used_samples();
    int used_b = hist_b.used_samples();

    // Print histograms next to each other with differences
    // First print header
    std::cout << "idx | Histogram A | Histogram B |  Difference \n";
    std::string div =  "----+-------------+-------------+-------------\n";
    std::cout << div;

    // Print values
    double diff_sum = 0;
    for (int i = 0; i < HistogramBase::size; i++)
    {
        double val_a = hist_a.bin(i);
        double val_b = hist_b.bin(i);
        double diff = val_a - val_b;
        diff_sum += diff;
        std::cout << std::setw(3) << i << " | "
                  << std::setw(11) << std::fixed << std::setprecision(5) << val_a << " | "
                  << std::setw(11) << val_b << " | "
                  << std::setw(12) << diff << "\n";
    }
    // Print footer
    std::cout << div;
    std::cout << "sum | " << std::fixed << std::setprecision(5)
              << std::setw(11) << hist_a.total_probabilities() << " | "
              << std::setw(11) << hist_b.total_probabilities() << " | "
              << std::setw(12) << diff_sum << "\n";
    std::cout << div;

    // Print statistics
    std::cout << "Pixels used:\nHistogram A: " << used_a
              << ", Histogram B: " << used_b
              << ", Ratio B to A: " << std::setprecision(5)
              << (float)used_b / used_a << "\n";
    std::cout << "Sum of squared differences: " << std::scientific
              << sum_of_squared_differences(hist_a, hist_b) << "\n";
    std::cout.flush();
}

std::vector<std::vector<ParamType>> make_permutations(std::vector<std::vector<ParamType>>& params)
{
    if (params.size() > 2)
        throw std::invalid_argument("Permutations of more than 2 dimensions not supported.");

    std::vector<std::vector<ParamType>> perms;

    if (params.size() == 1)
    {
        for (auto& x : params[0])
            perms.push_back(std::vector<ParamType>{x});
    }

    if (params.size() == 2)
    {
        for (auto& y : params[1])
        {
            for (auto& x : params[0])
                perms.push_back(std::vector<ParamType>{x, y});
        }
    }

    return perms;

}

void Evaluator::do_experiment(Experiment& experiment,
        const std::vector<std::string>& filenames,
        const std::vector<FullHistogram>& full_histograms)
{
    std::vector<std::unique_ptr<HistogramBase>> methods;
    std::vector<std::vector<double>> ssds;
    std::vector<std::vector<double>> scores;

    // Prepare parameters and methods
    auto param_permutations = make_permutations(experiment.params);
    for (auto& param_perm : param_permutations)
    {
        methods.push_back(make_class(experiment.method, param_perm));
    }
    ssds.resize(methods.size());
    scores.resize(methods.size());

    // Compute results for all images and parameters
    for (size_t i = 0; i < filenames.size(); i++)
    {
        GrayscaleImage img(filenames[i]);
        auto& full_hist = full_histograms[i];
        for (size_t j = 0; j < methods.size(); j++)
        {
            methods[j]->compute(img);
            ssds[j].push_back(sum_of_squared_differences(full_hist, *methods[j]));
            scores[j].push_back(score(full_hist, *methods[j]));
        }
    }

    // Summarize results
    for (size_t i = 0; i < methods.size(); i++)
    {
        ExperimentResult res;
        res.min_SSD = *std::min_element(ssds[i].begin(), ssds[i].end());
        res.max_SSD = *std::max_element(ssds[i].begin(), ssds[i].end());
        res.median_SSD = median(ssds[i]);
        res.min_score = *std::min_element(scores[i].begin(), scores[i].end());
        res.max_score = *std::max_element(scores[i].begin(), scores[i].end());
        res.median_score = median(scores[i]);
        experiment.results.push_back(res);
    }
}
