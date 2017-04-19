#include "evaluator.h"

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "undersampled_histogram.h"
#include "recursive_cross_histogram.h"
#include "concentric_histogram.h"
#include "chess_histogram.h"
#include "masked_histogram.h"
#include "random_sampling_histogram.h"
#include "nearest_neighbour_histogram.h"
#include "random_areas_histogram.h"

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
    case Methods::Circle: {
        int step = params.size() > 0 ? params[0].i : 2;

        return std::unique_ptr<HistogramBase>(new 
                ConcentricHistogram(step));
    }
    case Methods::Chess: {
        int step = params.size() > 0 ? params[0].i : 2;

        return std::unique_ptr<HistogramBase>(new 
                ChessHistogram(step));   
    }
    case Methods::Masked: {
        int step = params.size() > 0 ? params[0].i : 2;

        return std::unique_ptr<HistogramBase>(new 
                MaskedHistogram(step));   
    }
    case Methods::Random: {
        int numPixels = params.size() > 0 ? params[0].i : 10000;

        return std::unique_ptr<HistogramBase>(new
                RandomSamplingHistogram(numPixels));
    }
    case Methods::Neighbour: {
        int numPixels = params.size() > 0 ? params[0].i : 10000;

        return std::unique_ptr<HistogramBase>(new
                NearestNeighbourHistogram(numPixels));
    }
    case Methods::RandomAreas: {
        int numPixels = params.size() > 0 ? params[0].i : 10000;
        int areas = params.size() > 1 ? params[1].i : 2;

        return std::unique_ptr<HistogramBase>(new
                RandomAreasHistogram(numPixels, areas));
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

double pick_best(const ExperimentResult& r, bool scores)
{
    return scores ? r.max_score : r.min_SSD;
}

double pick_worst(const ExperimentResult& r, bool scores)
{
    return scores ? r.min_score : r.max_SSD;
}

double pick_median(const ExperimentResult& r, bool scores)
{
    return scores ? r.median_score : r.median_SSD;
}

void print_row_experiment(std::ostream &out, const Experiment& experiment, bool scores)
{
    auto& params = experiment.params[0];
    auto& results = experiment.results;
    int cell_width = 11;

    out << " param|";
    for (size_t i = 0; i < params.size(); i++) {
        out << std::setw(cell_width) << std::setprecision(4) << params[i].i
            << ((i < params.size()-1) ? "|" : "");
    }
    out<<"\n";

    out << "------+";
    std::string rs(cell_width, '-');
    for (size_t i = 0; i < params.size(); i++) {
        out << rs << ((i < params.size()-1) ? "+" : "");
    }
    out<<"\n";

    out << std::scientific << std::setprecision(4);
    for (int j = 0; j < 3; j++)
    {
        if (j == 0)
            out << "  best|";
        else if (j == 1)
            out << " worst|";
        else
            out << "median|";
        for (size_t i = 0; i < results.size(); i++) {
            out << std::setw(cell_width);
            if (j == 0)
                out << pick_best(results[i], scores);
            else if (j == 1)
                out << pick_worst(results[i], scores);
            else
                out << pick_median(results[i], scores);
            if (i < params.size()-1)
                out << "|";
        }
        out<<"\n";
    }
}

void print_table_experiment(std::ostream &out, const Experiment& experiment, bool scores)
{
    int cell_w = 11;
    auto& col_params = experiment.params[0];
    auto& row_params = experiment.params[1];
    auto& results = experiment.results;

    out << std::setprecision(4);

    std::string param_labels[2] = {"v param2", "param1 ->"};
    for (size_t i = 0; i < col_params.size()+2; i++) {
        if (i >= 2)
            out << std::setw(cell_w) << col_params[i-2].i;
        else
            out << std::string(cell_w - param_labels[i].size(), ' ') << param_labels[i];
        out << ((i < col_params.size()+1) ? "|" : "");
    }
    out<<"\n";

    std::string labels[3] = {"best", "worst", "median"};
    for (size_t y = 0; y < row_params.size(); y++)
    {
        std::string rs(cell_w, '-');
        for (size_t i = 0; i < col_params.size()+2; i++) {
            out << rs << ((i < col_params.size()+1) ? "+" : "");
        }
        out << "\n";

        for (int j = 0; j < 3; j++)
        {
            if (j == 1)
                out << std::setw(cell_w) << row_params[y].i;
            else
                out << std::string(cell_w, ' ');
            out << "|";

            out << std::string(cell_w - labels[j].size(), ' ') << labels[j] << "|";

            for (size_t x = 0; x < col_params.size(); x++)
            {
                int res_idx = x + y * col_params.size();
                out << std::setw(cell_w);
                if (j == 0)
                    out << pick_best(results[res_idx], scores);
                else if (j == 1)
                    out << pick_worst(results[res_idx], scores);
                else
                    out << pick_median(results[res_idx], scores);
                if (x < col_params.size()-1)
                    out << "|";
            }
            out << "\n";
        }
    }
}

void Evaluator::print_experiment(std::ostream& out, const Experiment& experiment, bool scores)
{
    // Method name header
    out << "##### "
        << make_class(experiment.method, std::vector<ParamType>())->to_string()
        << " (" << (scores ? "Score" : "Sum of Squared Differences") << ")"
        << " #####\n";
    if (experiment.params.size() == 1)
        print_row_experiment(out, experiment, scores);
    if (experiment.params.size() == 2)
        print_table_experiment(out, experiment, scores);

    out << std::endl;
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
