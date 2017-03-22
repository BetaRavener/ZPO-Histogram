#include "evaluator.h"

#include <iostream>
#include <iomanip>
#include <string>

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
