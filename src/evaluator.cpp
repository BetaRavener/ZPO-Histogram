#include "evaluator.h"

#include <iostream>
#include <iomanip>
#include <string>

void Evaluator::compare_histrograms(const HistogramBase& hist_a,
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
    int diff_sum = 0;
    for (int i = 0; i < HistogramBase::size; i++)
    {
        int val_a = hist_a.bin(i);
        int val_b = hist_b.bin(i);
        int diff = val_a - val_b;
        diff_sum += diff;
        std::cout << std::setw(3) << i << " | "
                  << std::setw(11) << val_a << " | "
                  << std::setw(11) << val_b << " | "
                  << std::setw(12) << diff << "\n";
    }
    // Print footer
    std::cout << div;
    std::cout << "sum | " << std::setw(11) << hist_a.samples() << " | "
              << std::setw(11) << hist_b.samples() << " | "
              << std::setw(12) << diff_sum << "\n";
    std::cout << div;

    // Print statistics
    std::cout << "Pixels used:\nHistogram A: " << used_a
              << "\nHistogram B: " << used_b
              << "\nRatio B to A: " << std::setprecision(5)
              << (float)used_b / used_a << "\n";
    std::cout.flush();
}
