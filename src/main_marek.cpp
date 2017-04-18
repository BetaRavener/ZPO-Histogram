#include "person.h"
#ifdef PERSON_MAREK

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
#include "chess_histogram.h"
#include "concentric_histogram.h"
#include "masked_histogram.h"
#include "evaluator.h"

using namespace std;


unique_ptr<HistogramBase> args_to_class(vector<string> params)
{
    if (params.size() < 1)
        throw std::invalid_argument("Missing method name.");

    Evaluator::Methods method;
    vector<ParamType> conv_params;
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
            throw invalid_argument("Error converting parameter to number.");
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
            throw invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Cross;
    }
    else if (params[0] == "circle")
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
            throw invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Circle;
    }
    else if (params[0] == "chess")
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
            throw invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Chess;
    }
    else if (params[0] == "masked")
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
            throw invalid_argument("Error converting parameter to number.");
        }
        method = Evaluator::Methods::Masked;
    }
    else
        throw invalid_argument("Invalid method: " + params[0] + ".");

    return Evaluator::make_class(method, conv_params);
}



void one_image(vector<string> params)
{
    string winName = "Window";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    if (params.size() < 1)
       throw invalid_argument("Missing image name parameter.");

    GrayscaleImage img(params[0]);
    GrayscaleImage img_sampled(img);
    img_sampled.scale_values(0.8);

    FullHistogram full_hist;
    auto other_hist = args_to_class(vector<string>(params.begin()+1, params.end()));

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
        cerr << "Missing mode parameter." << std::endl;
        return 1;
    }

    // Convert params into vector
    vector<string> params;
    for (int i = 2; i < argc; i++)
    {
        params.push_back(string(argv[i]));
    }
    
    try
    {
        string mode(argv[1]);
        if (mode == "image")
        { 
            one_image(params);
        }
        else if (mode == "method")
        {
           // one_method(params);
        }
        else if (mode == "all")
        {
           // all_methods(params);
        }
        else
            throw invalid_argument("Invalid mode parameter: " + mode + ".");
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
	return 0;
}
#endif
