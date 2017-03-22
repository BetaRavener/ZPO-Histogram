#include "person.h"
#ifdef PERSON_IVAN
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <fstream>
#include <iomanip>

#include <opencv2/highgui/highgui.hpp>

#include "histogram_base.h"
#include "grayscale_image.h"
#include "full_histogram.h"
#include "undersampled_histogram.h"
#include "recursive_cross_histogram.h"
#include "evaluator.h"

std::unique_ptr<HistogramBase> method_class(std::vector<std::string> params)
{
    if (params.size() < 1)
        throw std::invalid_argument("Missing method name.");

    if (params[0] == "downsample")
    {
        int step = 2;

        try
        {
            if (params.size() > 1)
                step = stoi(params[1]);
        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }
        return std::unique_ptr<HistogramBase>(new
                UndersampledHistogram(step));
    }
    else if (params[0] == "cross")
    {
        int threshold = 10;
        int forced_division_area = 0;

        try
        {
            if (params.size() > 1)
                threshold = stoi(params[1]);
            if (params.size() > 2)
                forced_division_area = stoi(params[2]);
        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }

        return std::unique_ptr<HistogramBase>(new
                RecursiveCrossHistogram(threshold, forced_division_area));
    }
    else
        throw std::invalid_argument("Invalid method: " + params[0] + ".");
}

void all_methods(std::vector<std::string> params)
{

}

void one_method(std::vector<std::string> params)
{
    if (params.size() < 1)
       throw std::invalid_argument("Missing image directory parameter.");
    if (params.size() < 2)
       throw std::invalid_argument("Missing output file parameter.");

    std::vector<std::string> filenames;
    cv::glob(params[0], filenames);
    if (filenames.size() == 0)
        throw std::invalid_argument("Image directory is empty.");

    FullHistogram full_hist;
    auto other_hist = method_class(std::vector<std::string>(params.begin()+2, params.end()));

    bool use_file = params[1] != "-";
    std::ofstream out_file;
    if (use_file)
        out_file.open(params[1]);
    std::ostream& output = use_file ? out_file : std::cout;

    std::vector<double> ssds;
    output << "Method: " << other_hist->to_string() << "\n";
    //SSD == Sum of squred differences (Least Squares)
    output << "Filename: SSD, pixel ratio\n";
    for (auto& filename : filenames)
    {
        GrayscaleImage img(filename);
        full_hist.compute(img);
        other_hist->compute(img);
        double ssd = Evaluator::sum_of_squared_differences(full_hist, *other_hist);
        ssds.push_back(ssd);
        output << filename << ": "
         << std::scientific << std::setprecision(5)
         << ssd << ", "
         << std::fixed
         << Evaluator::used_pixel_ratio(full_hist, *other_hist) << "\n";
    }
    output << "Statistics:\n Sum of squared differences:\n"
     << std::scientific << std::setprecision(3)
     << "  min: " << *std::min_element(ssds.begin(), ssds.end())
     << ", max: " << *std::max_element(ssds.begin(), ssds.end())
     << "\n  median: " << Evaluator::median(ssds) << "\n";
}

void one_image(std::vector<std::string> params)
{
    std::string winName = "Window";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    if (params.size() < 1)
       throw std::invalid_argument("Missing image name parameter.");

    GrayscaleImage img(params[0]);
    GrayscaleImage img_sampled(img);
    img_sampled.scale_values(0.8);

    FullHistogram full_hist;
    auto other_hist = method_class(std::vector<std::string>(params.begin()+1, params.end()));


    full_hist.compute(img);
    other_hist->compute(img, &img_sampled);

    Evaluator::compare_histrograms_text(full_hist, *other_hist);
    img_sampled.show(winName);
    cv::waitKey(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing mode parameter." << std::endl;
        return 1;
    }

    // Convert params into vector
    std::vector<std::string> params;
    for (int i = 2; i < argc; i++)
        params.push_back(std::string(argv[i]));

    try
    {
        std::string mode(argv[1]);
        if (mode == "image")
        {
            one_image(params);
        }
        else if (mode == "method")
        {
            one_method(params);
        }
        else if (mode == "all")
        {
            all_methods(params);
        }
        else
            throw std::invalid_argument("Invalid mode parameter: " + mode + ".");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }


	return 0;
}
#endif
