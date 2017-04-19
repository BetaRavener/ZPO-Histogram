#include "person.h"
#ifdef PERSON_MATEJ
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
#include "random_sampling_histogram.h"

std::unique_ptr<HistogramBase> args_to_class(std::vector<std::string> params)
{
    if (params.size() < 1)
        throw std::invalid_argument("Missing method name.");

    Evaluator::Methods method;
    std::vector<ParamType> conv_params;
    ParamType p;
    if (params[0] == "downsample")
    {
        try
        {
            if (params.size() > 1) {
                p.i = stoi(params[1]);
                conv_params.push_back(p);
            }
        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Downsample;

    }
    else if (params[0] == "cross")
    {
        try
        {
            if (params.size() > 1) {
                p.i = stoi(params[1]);
                conv_params.push_back(p);
            }

            if (params.size() > 2){
                p.i = stoi(params[2]);
                conv_params.push_back(p);
            }
        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Cross;
    }
    else if (params[0] == "random")
    {
        try
        {
            if (params.size() > 1) {
                p.i = stoi(params[1]);
                conv_params.push_back(p);
            }

        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Random;
    }
    else if (params[0] == "neighbour")
    {
        try
        {
            if (params.size() > 1) {
                p.i = stoi(params[1]);
                conv_params.push_back(p);
            }

        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Neighbour;
    }
    else if (params[0] == "randomAreas")
    {
        try
        {
            if (params.size() > 1) {
                p.i = stoi(params[1]);
                conv_params.push_back(p);
            }
            if (params.size() > 2) {
                p.i = stoi(params[2]);
                conv_params.push_back(p);
            }
        }
        catch(const std::exception& e)
        {
            throw std::invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::RandomAreas;
    }
    else
        throw std::invalid_argument("Invalid method: " + params[0] + ".");

    return Evaluator::make_class(method, conv_params);
}

void all_methods(std::vector<std::string> params)
{
    if (params.size() < 1)
       throw std::invalid_argument("Missing image directory parameter.");
    if (params.size() < 2)
       throw std::invalid_argument("Missing output file parameter.");

    bool scores = true;
    if (params.size() > 2)
    {
        if (params[2] == std::string("sum"))
            scores = false;
        else if (params[2] == std::string("score"))
            scores = true;
        else
            throw std::invalid_argument("Invalid result type.");
    }

    ParamType param;
    std::vector<std::string> filenames;
    cv::glob(params[0], filenames);
    //filenames=std::vector<std::string>{"img/lena.png"};
    std::vector<FullHistogram> full_histograms;

    bool use_file = params[1] != "-";
    std::ofstream out_file;
    if (use_file)
        out_file.open(params[1]);
    std::ostream& output = use_file ? out_file : std::cout;



    // Prepare full histograms for each image
//    for (auto& filename : filenames) {
//        GrayscaleImage img(filename);
//        full_histograms.push_back(FullHistogram());
//        full_histograms.back().compute(img);
//    }

    // Make experiment
//    { // Downsample experiment
//        Experiment experiment;
//        experiment.params.resize(1);
//        auto& down_steps = experiment.params[0];
//        for (int i = 2; i <= 6; i++) {
//            param.i = i;
//            down_steps.push_back(param);
//        }
//        experiment.method = Evaluator::Methods::Downsample;
//        Evaluator::do_experiment(experiment, filenames, full_histograms);
//        Evaluator::print_experiment(output, experiment, scores);
//    }

//    { // Cross experiment
//        Experiment experiment;
//        experiment.params.resize(2);
//        auto& thresholds = experiment.params[0];
//        auto& areas = experiment.params[1];
//        std::vector<int> threshold_values = {40,60,80,100,120};
//        std::vector<int> area_values = {0,100,400,800,1600};
//        for (auto val : threshold_values) {
//            param.i = val;
//            thresholds.push_back(param);
//        }
//        for (auto val : area_values) {
//            param.i = val;
//            areas.push_back(param);
//        }
//        experiment.method = Evaluator::Methods::Cross;
//        Evaluator::do_experiment(experiment, filenames, full_histograms);
//        Evaluator::print_experiment(output, experiment, scores);
//    }

    Experiment experiment;
    experiment.params.resize(2);
    experiment.method = Evaluator::Methods::Cross;
    auto& thresholds = experiment.params[0];
    auto& areas = experiment.params[1];
    std::vector<int> threshold_values = {40,60,80,100,120};
    std::vector<int> area_values = {0,100,400,800,1600};
    for (auto val : threshold_values) {
        param.i = val;
        thresholds.push_back(param);
    }
    for (auto val : area_values) {
        param.i = val;
        areas.push_back(param);
    }

    for (size_t i = 0; i < 25; i++)
    {
        ExperimentResult res;
        res.min_SSD = 0;
        res.max_SSD = i;
        res.median_SSD = 0.555555555555e-5;
        res.min_score = 5;
        res.max_score = 10;
        res.median_score = 7.5;
        experiment.results.push_back(res);
    }

    Evaluator::print_experiment(output, experiment, scores);
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
    auto other_hist = args_to_class(std::vector<std::string>(params.begin()+2, params.end()));

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
    auto other_hist = args_to_class(std::vector<std::string>(params.begin()+1, params.end()));


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

