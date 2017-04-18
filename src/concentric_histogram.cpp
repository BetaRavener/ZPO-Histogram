#include "concentric_histogram.h"

using namespace std;

ConcentricHistogram::ConcentricHistogram(int step) :
    _step(step)
{
}   


void ConcentricHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();
    int x1 = 0;
    int y1 = 0;
    int i;
    int j;
    
    int x2 = img.width() -1;
    int y2 = img.height() -1;
    
    if(_step >= 1)
    {        
        while(x1 <= x2 && y1 <= y2)
        {
          //---------------------------------
            for(i = x1; i <= x2; i++)
            {
                _data[img.pixel(i, y1)]++;
                _data[img.pixel(i, y2)]++; 
                _used_samples = _used_samples + 2;
                if(mark_img != nullptr)
                {
                      mark_img->pixel(i, y1, 255);
                      mark_img->pixel(i, y2, 255);       
                }          
            }
            
            for(j = y1; j <= y2; j++)
            {
                _data[img.pixel(x1,j)]++;
                _data[img.pixel(x2,j)]++; 
                _used_samples = _used_samples + 2;
                if(mark_img != nullptr)
                {
                    mark_img->pixel(x1, j, 255);
                    mark_img->pixel(x2, j, 255);
      
                }       
            }    
          //---------------------------------
            x1 += _step;
            y1 += _step;
            x2 -= _step;          
            y2 -= _step;                
        }  
    }

    // Normalize histogram
    for (int i = 0; i < size; i++)
    {
        _data[i] /= _used_samples;
    } 
}

std::string ConcentricHistogram::to_string(bool with_params) const
{
    return "Concentric Histogram" + (with_params ?
            (" (" + std::to_string(_step) + ")") : "");
}