#include "random_areas_histogram.h"
#include <random>
#include <iostream>
#include <functional>
#include <chrono>

RandomAreasHistogram::RandomAreasHistogram(int pixelPercent, int areas) :
    _pixelPercent(pixelPercent),
    _areas(areas)
{
}

void RandomAreasHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();

    int _widthAreas  = _areas;
    int _heightAreas = _areas;

    int width = img.width();
    int height = img.height();
    int numPixels = (width * height * _pixelPercent) / 100;
    int pixel; // 1D pixel address
    int x, y;
    std::vector<int> hodnoty;

    std::vector<int> widthStart(_widthAreas, 0);
    std::vector<int> widthEnd(_widthAreas, 0);
    std::vector<int> heightStart(_heightAreas, 0);
    std::vector<int> heightEnd(_heightAreas, 0);

    int numAreas = _widthAreas * _heightAreas;
    int imgNumPixels = numPixels / numAreas;
    std::vector<int> areaRange(numAreas, 0);

    double widthPart = (double)(width - 1) / (double)_widthAreas;
    double heightPart = (double)(height - 1) / (double)_heightAreas;


    for(int i = 0; i < _widthAreas; i++)
    {
        if(i == 0)
        {
            widthStart[0] = 0;
            widthEnd[0] = (int)llround(widthPart);
        }
        else
        {
            widthStart[i] = widthEnd[i - 1];
            widthEnd[i] = (int)llround(widthPart * (i + 1));
        }
    }

    for(int i = 0; i < _heightAreas; i++)
    {
        if(i == 0)
        {
            heightStart[0] = 0;
            heightEnd[0] = (int)llround(heightPart);
        }
        else
        {
            heightStart[i] = heightStart[i - 1];
            heightEnd[i] = (int)llround(heightPart * (i + 1));
        }
    }

    int min, max, pixVal, areaNumPixels;
    for(int i = 0; i < _heightAreas; i++)
    {
        for(int j = 0; j < _widthAreas; j++)
        {
            min = 255;
            max = 0;
            width = widthEnd[j] - widthStart[j];
            height = heightEnd[i] - heightStart[i];
            areaNumPixels = width * height;

            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            auto rand_num = std::bind(std::uniform_int_distribution<int>(0, areaNumPixels - 1), std::mt19937(seed));
            //std::mt19937::result_type seed = time(0);
            //auto rand_num = std::bind(std::uniform_int_distribution<int>(0, areaNumPixels - 1), std::mt19937(seed));
            for(int k = 0; k < imgNumPixels; k++)
            {
                //pixel = distribution(generator);
                pixel = rand_num();
                
                y = (pixel / width) + heightStart[i];
                x = (pixel % width) + widthStart[j];
                
                pixVal = img.pixel(x,y);
                if(pixVal < min)
                    min = pixVal;
                if(pixVal > max)
                    max = pixVal;
            }
            areaRange[i * _widthAreas + j] = max - min;
        }
    }

    int totalRange = 0;
    for (int i = 0; i < numAreas; i++)
    {
        totalRange += areaRange[i];
    }

    for(int i = 0; i < _heightAreas; i++)
    {
        for(int j = 0; j < _widthAreas; j++)
        {
            width = widthEnd[j] - widthStart[j];
            height = heightEnd[i] - heightStart[i];
            areaNumPixels = width * height;

            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            auto rand_num = std::bind(std::uniform_int_distribution<int>(0, areaNumPixels - 1), std::mt19937(seed));

            // other generators
            //std::mt19937::result_type seed = time(0);
            //auto rand_num = std::bind(std::uniform_int_distribution<int>(0, areaNumPixels - 1), std::mt19937(seed));
            //std::default_random_engine generator;
            //std::uniform_int_distribution<int> distribution(0, areaNumPixels - 1);
            imgNumPixels = (int)llround(((double)areaRange[i * _widthAreas + j] / (double)totalRange) * numPixels);
            for(int k = 0; k < imgNumPixels; k++)
            {
                //pixel = distribution(generator);
                pixel = rand_num();
                
                y = (pixel / width) + heightStart[i];
                x = (pixel % width) + widthStart[j];

                _used_samples++;
                _data[img.pixel(x,y)]++;
                if (mark_img != nullptr)
                    mark_img->pixel(x,y, 255);
            }
        }
    }


     // Normalize histogram
    for (int i = 0; i < size; i++)
        _data[i] /= _used_samples;
}

std::string RandomAreasHistogram::to_string(bool with_params) const
{
    return "Random Areas" +
            (with_params ? (" (" + std::to_string(_pixelPercent) + ", " +
            std::to_string(_areas) + ")") : "");
}
